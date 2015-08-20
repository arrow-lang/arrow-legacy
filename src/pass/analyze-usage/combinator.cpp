// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/analyze-usage.hpp"

namespace arrow {
namespace pass {

void AnalyzeUsage::visit_and(ast::And& x) {
  // Accept the left-hand-side (always entered)
  x.lhs->accept(*this);

  // Accept the right-hand-side (but apply its result as not-definite)
  _enter_block(*x.rhs);
  x.rhs->accept(*this);
  _exit_block(false);
}

void AnalyzeUsage::visit_or(ast::Or& x) {
  // Accept the left-hand-side (always entered)
  x.lhs->accept(*this);

  // Accept the right-hand-side (but apply its result as not-definite)
  _enter_block(*x.rhs);
  x.rhs->accept(*this);
  _exit_block(false);
}

}  // namespace pass
}  // namespace arrow
