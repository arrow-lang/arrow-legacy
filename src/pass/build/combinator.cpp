// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/util.hpp"
#include "arrow/pass/build.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void Build::do_combinator(ast::Binary& x) {
  // Resolve the type of the expression
  auto type = Resolve(_scope).run(x);
  if (!type) return;

  // Get the current insertion block and function
  auto current_block = LLVMGetInsertBlock(_ctx.irb);
  auto current_fn = LLVMGetBasicBlockParent(current_block);

  // Build the LHS
  auto lhs = Build(_ctx, _scope).run_scalar(*x.lhs);
  if (!lhs) return;

  // Create the three neccessary basic blocks: then, else, merge.
  auto then_b = LLVMAppendBasicBlock(current_fn, "");
  auto else_b = LLVMAppendBasicBlock(current_fn, "");
  auto merge_b = LLVMAppendBasicBlock(current_fn, "");

  // Create the conditional branch.
  auto is_and = typeid(x) == typeid(ast::And);
  if (is_and) {
      LLVMBuildCondBr(_ctx.irb, lhs->get_value(_ctx), then_b, else_b);
  } else {
      LLVMBuildCondBr(_ctx.irb, lhs->get_value(_ctx), else_b, then_b);
  }

  // Switch to the `then` block.
  LLVMPositionBuilderAtEnd(_ctx.irb, then_b);

  // Build the LHS
  auto rhs = Build(_ctx, _scope).run_scalar(*x.rhs);
  if (!rhs) return;

  // Create a branch to the `merge` block.
  LLVMBuildBr(_ctx.irb, merge_b);

  // Insert the `else` block after our current block.
  then_b = LLVMGetInsertBlock(_ctx.irb);
  LLVMMoveBasicBlockAfter(else_b, then_b);

  // Switch to the `else` block.
  LLVMPositionBuilderAtEnd(_ctx.irb, else_b);

  // Create a branch to the `merge` block.
  LLVMBuildBr(_ctx.irb, merge_b);

  // Insert the `else` block after our current block.
  else_b = LLVMGetInsertBlock(_ctx.irb);
  LLVMMoveBasicBlockAfter(merge_b, else_b);

  // Switch to the `merge` block.
  LLVMPositionBuilderAtEnd(_ctx.irb, merge_b);

  // Create a `PHI` node.
  auto bool_val = LLVMConstInt(type->handle(), is_and ? 0 : 1, false);
  auto res = LLVMBuildPhi(_ctx.irb, type->handle(), "");
  auto rhs_val = rhs->get_value(_ctx);
  LLVMAddIncoming(res, &rhs_val, &then_b, 1);
  LLVMAddIncoming(res, &bool_val, &else_b, 1);

  // Build and push the resultant value
  _stack.push_front(new code::Value(res, type));
}

void Build::visit_and(ast::And& x) {
  do_combinator(x);
}

void Build::visit_or(ast::Or& x) {
  do_combinator(x);
}

}  // namespace pass
}  // namespace arrow
