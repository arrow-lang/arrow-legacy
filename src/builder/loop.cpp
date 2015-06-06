// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/generator.hpp"
#include "arrow/builder.hpp"
#include "arrow/resolver.hpp"

using arrow::Builder;
using arrow::resolve;

void Builder::visit_loop(ast::Loop& x) {
  // Push a new scope level
  code::Scope scope{_cs};

  // Get the current insertion block and function
  auto current_block = LLVMGetInsertBlock(_g._irb);
  auto current_fn = LLVMGetBasicBlockParent(current_block);

  // Create blocks to contain statements
  LLVMBasicBlockRef loop_block = nullptr;
  LLVMBasicBlockRef cond_block = nullptr;
  LLVMBasicBlockRef merge_block = nullptr;

  // If we have a condition ..
  auto bool_ = std::static_pointer_cast<code::Type>(_scope.get("bool"));
  if (x.condition) {
    // Create condition and merge blocks
    cond_block = LLVMAppendBasicBlock(current_fn, "");
    loop_block = LLVMAppendBasicBlock(current_fn, "");
    merge_block = LLVMAppendBasicBlock(current_fn, "");

    // Build the condition expression
    LLVMPositionBuilderAtEnd(_g._irb, cond_block);
    auto cond = build_scalar_of<code::Value>(*x.condition, &scope);
    if (!cond) return;

    // Cast the condition expression to boolean
    cond = cond->cast(_g, *x.condition, bool_);
    if (!cond) return;

    // Build the conditional branch
    LLVMBuildCondBr(
      _g._irb, cond->value_of(_g), loop_block, merge_block);
  } else {
    loop_block = LLVMAppendBasicBlock(current_fn, "");
    cond_block = loop_block;
  }

  // Create the un-conditional jump to the "condition"
  LLVMPositionBuilderAtEnd(_g._irb, current_block);
  LLVMBuildBr(_g._irb, cond_block);

  // Build each statement in the loop body
  LLVMPositionBuilderAtEnd(_g._irb, loop_block);
  do_sequence(x.sequence, &scope);

  // Jump back to the "condition"
  LLVMBuildBr(_g._irb, cond_block);

  // If we have a merge block (not an inf. loop) ..
  if (merge_block) {
    // Switch our context to it (and move it to the end)
    LLVMMoveBasicBlockAfter(merge_block, LLVMGetInsertBlock(_g._irb));
    LLVMPositionBuilderAtEnd(_g._irb, merge_block);
  }
}
