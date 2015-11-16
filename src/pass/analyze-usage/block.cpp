// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/analyze-usage.hpp"
#include "arrow/match.hpp"

namespace arrow {
namespace pass {

void AnalyzeUsage::_enter_block(ast::Node& x) {
  // Enter the <anonymous> scope block ..
  _scope->enter(&x);
}

void AnalyzeUsage::_exit_block(bool is_definite) {
  // Grab our current block
  auto current = _scope->top();

  // Exit the <anonymous> scope block
  _scope->exit();

  // If we are not yet at the top ..
  auto top = _scope->top();
  if (top != nullptr) {
    // Move all variable assignments up-one ..
    for (auto& item : _assign[current]) {
      if (!current->find(item->context, false)) {
        // Non-local assignment; push
        auto is_assigned = item->is_assigned(current);
        item->add_assignment(top, is_definite && *is_assigned);
      }
    }
  }
}

void AnalyzeUsage::visit_block(ast::Block& x) {
  // Enter the <anonymous> scope block ..
  _enter_block(x);

  // Run the base method (which iterates over the statements)
  Visitor::visit_block(x);

  // Exit the <anonymous> scope block
  _exit_block();
}

}  // namespace pass
}  // namespace arrow
