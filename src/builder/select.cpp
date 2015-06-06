// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/generator.hpp"
#include "arrow/builder.hpp"
#include "arrow/resolver.hpp"

using arrow::Builder;
using arrow::resolve;

void Builder::visit_select(ast::Select& x) {
  // Push a new scope level
  code::Scope scope{"", _cs};

  // Get the current insertion block and function
  auto current_block = LLVMGetInsertBlock(_g._irb);
  auto current_fn = LLVMGetBasicBlockParent(current_block);

  std::vector<LLVMBasicBlockRef> blocks;
  std::vector<LLVMValueRef> values;
  std::vector<LLVMBasicBlockRef> value_blocks;
  unsigned index = 0;
  bool has_value = true;
  auto bool_ = std::static_pointer_cast<code::Type>(_scope.get("bool"));
  for (; index < x.branches.size(); ++index) {
    auto& br = x.branches.at(index);

    // The last branch (else) is signaled by having no condition.
    if (!br->condition) { index -= 1; break; }

    // Build the condition expression
    // TODO: If statements should have their own scope
    auto cond = build_scalar_of<code::Value>(*br->condition, &scope);
    if (!cond) return;

    // Cast the condition expression to boolean
    cond = cond->cast(_g, *br->condition, bool_);
    if (!cond) return;

    // Create the THEN and NEXT blocks
    auto then_block = LLVMAppendBasicBlock(current_fn, "");
    auto next_block = LLVMAppendBasicBlock(current_fn, "");

    // Build the conditional branch
    LLVMBuildCondBr(
      _g._irb, cond->value_of(_g), then_block, next_block);

    // Activate the THEN block
    LLVMPositionBuilderAtEnd(_g._irb, then_block);

    // TODO: Extract into `do_select_branch` or something
    // Build each statement
    auto last = std::dynamic_pointer_cast<code::Value>(
      do_sequence(br->sequence, &scope));
    auto block = LLVMGetInsertBlock(_g._irb);

    if (has_value && last) {
      // Append to the value chain
      values.push_back(last->value_of(_g));
      value_blocks.push_back(block);
    } else if (!LLVMGetBasicBlockTerminator(block)) {
      // This block wasn't terminated and it has no value
      // We no longer have a value
      has_value = false;
    }

    // Append to the block chain
    blocks.push_back(block);

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

    // TODO: Extract into `do_select_branch` or something
    // Build each statement
    auto last = std::dynamic_pointer_cast<code::Value>(
      do_sequence(br->sequence, &scope));
    auto block = LLVMGetInsertBlock(_g._irb);

    if (has_value && last) {
      // Append to the value chain
      values.push_back(last->value_of(_g));
      value_blocks.push_back(block);
    } else if (!LLVMGetBasicBlockTerminator(block)) {
      // This block wasn't terminated and it has no value
      // We no longer have a value
      has_value = false;
    }

    // Append to the block chain
    blocks.push_back(block);

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

  // If we still have a value ..
  // TODO: No way to signal that we /need/ a value so no way
  //  to reliably produce an error message saying we didn't get one.
  if (has_value) {
    // Push our PHI node
    auto type = resolve(_g, scope, x);
    if (!type) { return; }
    auto res = LLVMBuildPhi(_g._irb, type->handle(), "");
    LLVMAddIncoming(res, values.data(), value_blocks.data(), values.size());

    _stack.push(std::make_shared<code::Value>(res, type));
  }
}
