// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <algorithm>

#include "arrow/match.hpp"
#include "arrow/pass/analyze-usage.hpp"

namespace arrow {
namespace pass {

void AnalyzeUsage::visit_select(ast::Select& x) {
  std::vector<Ref<code::Block>> blocks;
  unsigned idx = 0;

  for (auto& br : x.branches) {
    if (idx > 0 && br->condition.is<ast::Block>()) {
      // Enter the <anonymous> scope block
      _enter_block(*br->condition.as<ast::Block>());

      // Accept the condition ..
      br->condition->accept(*this);

      // Exit the block and forget what happened
      _exit_block(false);
    } else {
      // Accept the condition (without forgetting about it)
      br->condition->accept(*this);
    }

    // Enter the <anonymous> scope block
    _enter_block(*br->block);

    // Run the base method (which iterates over the statements)
    Visitor::visit_block(*br->block);

    // Remember the block
    blocks.push_back(_scope->top());

    // Exit the <anonymous> scope block
    _scope->exit();

    idx += 1;
  }

  if (x.else_block) {
    // Enter the <anonymous> scope block
    _enter_block(*x.else_block);

    // Run the base method (which iterates over the statements)
    Visitor::visit_block(*x.else_block);

    // Remember the block
    blocks.push_back(_scope->top());

    // Exit the <anonymous> scope block
    _scope->exit();
  }

  // Compile a list of slots that have been (possibly) assigned
  std::unordered_map<ast::Node*, Ref<code::Slot>> slots;
  std::map<ast::Node*, int> block_assign;

  for (auto& block : blocks) {
    for (auto& item : _assign[block]) {
      if (slots.find(item->context) == slots.end()) {
        slots[item->context] = item;
        block_assign[item->context] = 0;
      }

      if (block_assign[item->context] == -1) {
        // We've been previously marked as possibly assigned ..
        continue;
      }

      auto is_assigned = item->is_assigned(block);
      if (is_assigned && *is_assigned) {
        block_assign[item->context] += 1;
      } else if (!*is_assigned) {
        block_assign[item->context] = -1;
      }
    }
  }

  // Iterate through the non-local assignments (in total)
  int required = (x.branches.size() + 1);
  for (auto& item : block_assign) {
    auto slot = slots[item.first];
    slot->add_assignment(_scope->top(), item.second == required);
    _assign[_scope->top()].push_back(slot);
  }
}

}  // namespace pass
}  // namespace arrow
