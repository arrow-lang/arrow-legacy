// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <algorithm>

#include "arrow/pass/analyze-usage.hpp"
#include "arrow/match.hpp"

namespace arrow {
namespace pass {

void AnalyzeUsage::visit_loop(ast::Loop& x) {
  if (x.condition) {
    // Enter the <anonymous> scope block
    _enter_block(*(x.condition));

    // Accept the condition (xN) ..
    x.condition->accept(*this);
    if (Log::get().count("error") > 0) return;

    x.condition->accept(*this);
    if (Log::get().count("error") > 0) return;

    // Exit the block and forget what happened
    _exit_block(false);
  }

  // Enter the <anonymous> scope block
  _enter_block(*x.block);

  // Run the base method (which iterates over the statements) [x2]
  Visitor::visit_block(*x.block);
  if (Log::get().count("error") > 0) return;

  Visitor::visit_block(*x.block);
  if (Log::get().count("error") > 0) return;

  // Exit (and forget) the <anonymous> scope block
  _exit_block(false);
}

}  // namespace pass
}  // namespace arrow
