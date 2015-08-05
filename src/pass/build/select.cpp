// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/util.hpp"
#include "arrow/pass/build.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void Build::visit_select(ast::Select& x) {
  // Get the current LLVM block and function
  auto current_block = LLVMGetInsertBlock(_ctx.irb);
  auto current_fn = LLVMGetBasicBlockParent(current_block);

  std::vector<LLVMBasicBlockRef> blocks;
  std::vector<LLVMValueRef> values;
  std::vector<LLVMBasicBlockRef> value_blocks;

  // Resolve the type of us (in full; only used if we have a value)
  auto type = Resolve(_scope).run(x);

  // A Select expression has a value IIF it has an else and
  // each of its branches have a value
  bool has_value = !!type;

  auto do_select_branch = [&](ast::Block& block) -> bool {
    // Build the block
    auto value = Build(_ctx, _scope).run_scalar(block);
    auto iblock = LLVMGetInsertBlock(_ctx.irb);

    if (has_value && value) {
      // Cast the value to the type analyzed result
      value = util::cast(_ctx, value, block, type, false);
      if (!value) return false;

      // Append to the value chain
      values.push_back(value->get_value(_ctx));
      value_blocks.push_back(iblock);
    } else if (!LLVMGetBasicBlockTerminator(iblock)) {
      // This block wasn't terminated and it has no value
      // We no longer have a value
      has_value = false;
    }

    // Append to the block chain
    blocks.push_back(iblock);

    return true;
  };

  // Iterate through each branch and build its contained block ..
  for (auto& br : x.branches) {
    // Build the condition expression
    auto cond = Build(_ctx, _scope).run_scalar(*br->condition);
    if (!cond) return;

    // Create the THEN and NEXT LLVM blocks
    auto then_block = LLVMAppendBasicBlock(current_fn, "");
    auto next_block = LLVMAppendBasicBlock(current_fn, "");

    // Build the conditional branch
    LLVMBuildCondBr(_ctx.irb, cond->get_value(_ctx), then_block, next_block);

    // Activate the THEN block
    LLVMPositionBuilderAtEnd(_ctx.irb, then_block);

    // Process the branch ..
    if (!do_select_branch(*br->block)) return;

    // Insert the `next` block after our current block.
    LLVMMoveBasicBlockAfter(next_block, LLVMGetInsertBlock(_ctx.irb));

    // Replace the outer-block with our new "merge" block.
    LLVMPositionBuilderAtEnd(_ctx.irb, next_block);
  }

  // Check for and build the else_block
  LLVMBasicBlockRef merge_block;
  if (x.else_block) {
    // Process the branch ..
    do_select_branch(*x.else_block);

    // Create the final "merge" block
    merge_block = LLVMAppendBasicBlock(current_fn, "");
  } else {
    // Use the elided "else" block as the "merge" block
    merge_block = LLVMGetLastBasicBlock(current_fn);
  }

  // Iterate through the established branches and have them return to
  // the "merge" block (if they are not otherwise terminated).
  unsigned term = 0;
  for (auto& ib : blocks) {
    if (!LLVMGetBasicBlockTerminator(ib)) {
      // Insert the non-conditional branch.
      LLVMPositionBuilderAtEnd(_ctx.irb, ib);
      LLVMBuildBr(_ctx.irb, merge_block);
    } else {
      term += 1;
    }
  }

  // If all blocks were terminated and there is an ELSE present;
  // remove the merge block
  if (term == blocks.size() && x.else_block) {
    LLVMDeleteBasicBlock(merge_block);
  }

  // Re-establish our insertion point.
  LLVMPositionBuilderAtEnd(_ctx.irb, merge_block);

  // If we still have a value ..
  if (has_value) {
    // Make the PHI value
    auto res = LLVMBuildPhi(_ctx.irb, type->handle(), "");
    LLVMAddIncoming(res, values.data(), value_blocks.data(), values.size());
    Ref<code::Value> res_value = new code::Value(res, type);

    _stack.push_front(res_value);
  }
}

}  // namespace pass
}  // namespace arrow
