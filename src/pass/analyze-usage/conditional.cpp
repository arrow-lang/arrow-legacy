// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <algorithm>

#include "arrow/pass/analyze-usage.hpp"
#include "arrow/match.hpp"

namespace arrow {
namespace pass {

void AnalyzeUsage::visit_conditional(ast::Conditional& x) {
  // Accept the conditiona (always entered)
  x.condition->accept(*this);

  // Accept the lhs-hand-side (but apply its result as not-definite)
  _enter_block(*x.lhs);
  x.lhs->accept(*this);
  _exit_block(false);

  // Accept the rhs-hand-side (but apply its result as not-definite)
  _enter_block(*x.rhs);
  x.rhs->accept(*this);
  _exit_block(false);
}

}  // namespace pass
}  // namespace arrow
