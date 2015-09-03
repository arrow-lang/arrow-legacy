// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <algorithm>

#include "arrow/pass/build.hpp"

namespace arrow {
namespace pass {

void Build::visit_loop(ast::Loop& x) {
  // Get the current insertion block and function
  auto current_block = LLVMGetInsertBlock(_ctx.irb);
  auto current_fn = LLVMGetBasicBlockParent(current_block);

  // Create blocks to contain statements
  LLVMBasicBlockRef loop_block = nullptr;
  LLVMBasicBlockRef cond_block = nullptr;
  LLVMBasicBlockRef merge_block = nullptr;

  // If we have a condition ..
  if (x.condition) {
    // Create condition and merge blocks
    cond_block = LLVMAppendBasicBlock(current_fn, "loop-cond");
    loop_block = LLVMAppendBasicBlock(current_fn, "loop-body");
    merge_block = LLVMAppendBasicBlock(current_fn, "loop-merge");

    // Build the condition expression
    LLVMPositionBuilderAtEnd(_ctx.irb, cond_block);
    auto cond = Build(_ctx, _scope).run_scalar(*x.condition);
    if (!cond) return;

    // Build the conditional branch
    LLVMBuildCondBr(_ctx.irb, cond->get_value(_ctx), loop_block, merge_block);

  } else {
    loop_block = LLVMAppendBasicBlock(current_fn, "loop-body");
    merge_block = LLVMAppendBasicBlock(current_fn, "loop-merge");
    cond_block = loop_block;
  }

  // Push a new LoopFrame
  _ctx.loops.push({cond_block,  merge_block});

  // Create the un-conditional jump to the "condition"
  LLVMMoveBasicBlockAfter(cond_block, current_block);
  LLVMPositionBuilderAtEnd(_ctx.irb, current_block);
  LLVMBuildBr(_ctx.irb, cond_block);

  // Build each statement in the loop body
  LLVMPositionBuilderAtEnd(_ctx.irb, loop_block);
  x.block->accept(*this);

  // Update our block reference (effectively flattening any blocks that
  // that were made)
  loop_block = LLVMGetInsertBlock(_ctx.irb);

  if (!LLVMGetBasicBlockTerminator(loop_block)) {
    // Jump back to the "condition"
    LLVMBuildBr(_ctx.irb, cond_block);
  }

  // Switch our context to it (and move it to the end)
  LLVMMoveBasicBlockAfter(merge_block, LLVMGetInsertBlock(_ctx.irb));
  LLVMPositionBuilderAtEnd(_ctx.irb, merge_block);
  _ctx.loops.pop();
}

void Build::visit_break(ast::Break&) {
  auto cur = _ctx.loops.top();
  LLVMBuildBr(_ctx.irb, cur.merge);
}

void Build::visit_continue(ast::Continue&) {
  auto cur = _ctx.loops.top();
  LLVMBuildBr(_ctx.irb, cur.condition);
}

}  // namespace pass
}  // namespace arrow
