// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <algorithm>

#include "arrow/match.hpp"
#include "arrow/pass/analyze-usage.hpp"

namespace arrow {
namespace pass {

void AnalyzeUsage::visit_select(ast::Select& x) {
  std::vector<code::Block*> blocks;
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
    blocks.push_back(_scope->top().get());

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
    blocks.push_back(_scope->top().get());

    // Exit the <anonymous> scope block
    _scope->exit();
  }

  // Iterate through each branchs' block ..
  std::unordered_map<ast::Node*, int> set;
  for (auto& block : blocks) {
    // Iterate through it's non-local variables
    for (auto& nl : _non_local_assign[block]) {
      // We know this non-local was assigned; but was it definitely assigned?
      bool is_definite = false;
      for (auto& assign : _assign[block][nl]) {
        if (assign.is_definite) {
          is_definite = true;
          break;
        }
      }

      if (is_definite) {
        if (set.find(nl) == set.end()) {
          set[nl] = 1;
        } else if (set[nl] >= 0) {
          set[nl] += 1;
        }
      } else {
        set[nl] = -1;
      }
    }
  }

  // Iterate through the non-local assignments (in total)
  auto top = _scope->top();
  auto& top_assign = _assign[top.get()];
  int required = (x.branches.size() + 1);
  for (auto& item : set) {
    top_assign[item.first].emplace_back(item.second == required);

    // Are we still non-local.. ?
    if (!top->contains(item.first, false)) {
      _non_local_assign[top.get()].insert(item.first);
    }
  }
}

}  // namespace pass
}  // namespace arrow
