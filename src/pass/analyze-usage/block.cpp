// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/pass/analyze-usage.hpp"

namespace arrow {
namespace pass {

void AnalyzeUsage::_enter_block(ast::Block& x) {
  // Enter the <anonymous> scope block ..
  _scope->enter(&x);

  // auto prev_block = _scope->top();
  // auto prev = prev_block ? prev_block.get() : nullptr;
  // auto bp = block.get();
  //
  // // Intialize the assign-sets (if needed)
  // if (_assign.find(bp) == _assign.end()) {
  //   if (prev) {
  //     _assign[bp] = _assign[prev];
  //   } else {
  //     _assign[bp] = {};
  //   }
  // }
  //
  // if (_non_local_assign.find(bp) == _non_local_assign.end()) {
  //   _non_local_assign[bp] = {};
  // }
}

void AnalyzeUsage::_exit_block(bool is_definite) {
  // Grab our current block
  // auto bp = _scope->top().get();

  // Exit the <anonymous> scope block
  _scope->exit();

  // If we are not yet at the top ..
  // auto top = _scope->top();
  // if (top != nullptr) {
  //   // Move all non-local variable assignments up-one ..
  //   for (auto& nl : _non_local_assign[bp]) {
  //     auto& ref = _assign[top.get()][nl];
  //     bool is_assigned = false;
  //     for (auto& assign : _assign[bp][nl]) {
  //       if (assign.is_definite) {
  //         is_assigned = true;
  //         break;
  //       }
  //     }
  //
  //     if (is_assigned && is_definite) {
  //       ref.emplace_back(true);
  //     } else {
  //       ref.emplace_back(false);
  //     }
  //
  //     // Are we still non-local.. ?
  //     if (!top->contains(nl, false)) {
  //       _non_local_assign[top.get()].insert(nl);
  //     }
  //   }
  // }
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
