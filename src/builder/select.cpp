// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/generator.hpp"
#include "arrow/builder.hpp"
#include "arrow/resolver.hpp"

using arrow::Builder;
using arrow::resolve;


// 1. Build condition expression
// 2. Create a THEN block and an ELSE block
// 3. Create a conditional branch that decides between the THEN block and the ELSE block
// 4. Build sequence into the THEN block


void Builder::visit_select(ast::Select& x) {
  // Get the current insertion block and function
  auto current_block = LLVMGetInsertBlock(_g._irb);
  auto current_fn = LLVMGetBasicBlockParent(current_block);

  std::vector<LLVMBasicBlockRef> blocks;
  unsigned index = 0;
  for (; index < x.branches.size(); ++index) {
    auto& br = x.branches.at(index);

    // The last branch (else) is signaled by having no condition.
    if (!br->condition) { index -= 1; break; }

    // Build the condition expression
    // TODO: If statements should have their own scope
    auto cond = build_scalar_of<code::Value>(*br->condition, _cs);
    if (!cond) return;

    // Create the THEN and NEXT blocks
    auto then_block = LLVMAppendBasicBlock(current_fn, "");
    auto next_block = LLVMAppendBasicBlock(current_fn, "");

    // Build the conditional branch
    LLVMBuildCondBr(
      _g._irb, cond->value_of(_g), then_block, next_block);

    // Activate the THEN block
    LLVMPositionBuilderAtEnd(_g._irb, then_block);

    // Build each statement
    // TODO: This should be a utility (build_sequence)
    for (auto& item : br->sequence) {
      // Remember the size of the stack at this point (so we can
      // detect if an item gets pushed; and then remove it)
      auto cnt = _stack.size();

      // TODO: If statements should have their own scope
      build(*item, _cs);

      // Remove anything pushed onto the stack
      for (unsigned i = 0; i < (_stack.size() - cnt); ++i) {
        _stack.pop();
      }
    }

    // Append to the block chain
    blocks.push_back(LLVMGetInsertBlock(_g._irb));

    // Insert the `next` block after our current block.
    LLVMMoveBasicBlockAfter(next_block, LLVMGetInsertBlock(_g._irb));

    // Replace the outer-block with our new "merge" block.
    LLVMPositionBuilderAtEnd(_g._irb, next_block);
  }

  // Use the last elided block for our final "else" block.
  LLVMBasicBlockRef merge_block;
  if (index < x.branches.size()) {
    // We still have an "else" branch left
    auto& br = x.branches[index + 1];

    // Build each statement
    // TODO: This should be a utility (build_sequence)
    for (auto& item : br->sequence) {
      // Remember the size of the stack at this point (so we can
      // detect if an item gets pushed; and then remove it)
      auto cnt = _stack.size();

      // TODO: If statements should have their own scope
      build(*item, _cs);

      // Remove anything pushed onto the stack
      for (unsigned i = 0; i < (_stack.size() - cnt); ++i) {
        _stack.pop();
      }
    }

    // Append to the block chain
    blocks.push_back(LLVMGetInsertBlock(_g._irb));

    // Create the final "merge" block
    merge_block = LLVMAppendBasicBlock(current_fn, "");
  } else {
    // Use the elided "else" block as the "merge" block
    merge_block = LLVMGetLastBasicBlock(current_fn);
  }

  // Iterate through the established branches and have them return to
  // the "merge" block (if they are not otherwise terminated).
  unsigned term = 0;
  for (auto& block : blocks) {
    if (!LLVMGetBasicBlockTerminator(block)) {
      // Insert the non-conditional branch.
      LLVMPositionBuilderAtEnd(_g._irb, block);
      LLVMBuildBr(_g._irb, merge_block);
    } else {
      term += 1;
    }
  }

  // If all blocks were terminated and there is an ELSE present;
  // remove the merge block
  if (term == blocks.size()) {
    if (!(x.branches[x.branches.size() - 1]->condition)) {
      LLVMDeleteBasicBlock(merge_block);
    }
  }

  // Re-establish our insertion point.
  LLVMPositionBuilderAtEnd(_g._irb, merge_block);
}
