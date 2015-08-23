// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/util.hpp"
#include "arrow/pass/build.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void Build::visit_conditional(ast::Conditional& x) {
  // Resolve the type of the expression
  auto type = Resolve(_scope).run(x);
  if (!type) return;

  // Get the current insertion block and function
  auto current_block = LLVMGetInsertBlock(_ctx.irb);
  auto current_fn = LLVMGetBasicBlockParent(current_block);

  // Build the condition
  auto condition = Build(_ctx, _scope).run_scalar(*x.condition);
  if (!condition) return;

  // Create the three neccessary basic blocks: then, else, merge.
  auto then_b = LLVMAppendBasicBlock(current_fn, "");
  auto else_b = LLVMAppendBasicBlock(current_fn, "");
  auto merge_b = LLVMAppendBasicBlock(current_fn, "");

  // Create the conditional branch.
  LLVMBuildCondBr(_ctx.irb, condition->get_value(_ctx), then_b, else_b);

  // Switch to the `then` block.
  LLVMPositionBuilderAtEnd(_ctx.irb, then_b);

  // Build the LHS
  auto lhs = Build(_ctx, _scope).run_scalar(*x.lhs);
  if (!lhs) return;

  // Cast the LHS to the target type
  lhs = util::cast(_ctx, lhs, *x.lhs, type, false);
  if (!lhs) return;

  // Create a branch to the `merge` block.
  LLVMBuildBr(_ctx.irb, merge_b);

  // Insert the `else` block after our current block.
  then_b = LLVMGetInsertBlock(_ctx.irb);
  LLVMMoveBasicBlockAfter(else_b, then_b);

  // Switch to the `else` block.
  LLVMPositionBuilderAtEnd(_ctx.irb, else_b);

  // Build the RHS
  auto rhs = Build(_ctx, _scope).run_scalar(*x.rhs);
  if (!rhs) return;

  // Cast the LHS to the target type
  rhs = util::cast(_ctx, rhs, *x.rhs, type, false);
  if (!rhs) return;

  // Create a branch to the `merge` block.
  LLVMBuildBr(_ctx.irb, merge_b);

  // Insert the `merge` block after our current block.
  else_b = LLVMGetInsertBlock(_ctx.irb);
  LLVMMoveBasicBlockAfter(merge_b, else_b);

  // Switch to the `merge` block.
  LLVMPositionBuilderAtEnd(_ctx.irb, merge_b);

  // Create a `PHI` node.
  auto res = LLVMBuildPhi(_ctx.irb, type->handle(), "");
  auto lhs_val = lhs->get_value(_ctx);
  auto rhs_val = rhs->get_value(_ctx);
  LLVMAddIncoming(res, &lhs_val, &then_b, 1);
  LLVMAddIncoming(res, &rhs_val, &else_b, 1);

  // Build and push the resultant value
  _stack.push_front(new code::Value(res, type));
}

}  // namespace pass
}  // namespace arrow
