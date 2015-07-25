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
    // Accept the condition
    if (idx > 0 && br->condition.is<ast::Block>()) {
      // Enter the <anonymous> scope block
      _enter_block(*br->condition.as<ast::Block>());

      // Run the base method (which iterates over the statements)
      Visitor::visit_block(*br->condition.as<ast::Block>());

      // Exit the block and forget what happened
      _exit_block(false);
    } else {
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
  std::unordered_map<code::Slot*, int> block_assign;
  for (auto& block : blocks) {
    for (auto& item : _assign[block]) {
      if (block_assign.find(item.get()) != block_assign.end() &&
          block_assign[item.get()] == -1) {
        // If we've already been marked as possibly assigned;
        // get out
        continue;
      }

      auto is_assigned = item->is_assigned(block);
      if (is_assigned && *is_assigned) {
        block_assign[item.get()] += 1;
      } else if (!*is_assigned) {
        block_assign[item.get()] = -1;
      }
    }
  }

  // Iterate through the non-local assignments (in total)
  int required = (x.branches.size() + 1);
  for (auto& item : block_assign) {
    item.first->add_assignment(_scope->top(), item.second == required);
  }
}

}  // namespace pass
}  // namespace arrow
