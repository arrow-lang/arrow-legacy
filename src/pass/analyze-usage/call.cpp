// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/pass/analyze-usage.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void AnalyzeUsage::do_realize_function(
  ast::Node& x, code::TypeFunction& fn, bool is_definite
) {
  // Use: Iterate through each non-local initial-use and ensure
  //      that the variables in question are ready-to-use now
  // TODO(_): Should we use the span from the item use or the call?
  for (auto& item : fn._use) {
    do_use(x, *item);
  }

  if (is_definite) {
    // Assign: Iterate through each non-local assign and ensure
    //         that the variables in question are ready-to-assign (and mark
    //         them as assigned).
    // TODO(_): Should we use the span from the item use or the call?
    for (auto& ref : fn._assign) {
      do_assign(x, ref.first, ref.second);
    }
  }
}

void AnalyzeUsage::visit_call(ast::Call& x) {
  // Run the base method (analyze the operand and each argument)
  Visitor::visit_call(x);
  if (Log::get().count("error") > 0) return;

  // Resolve the function-type of the operand
  auto type = Resolve(_scope).run(*x.operand);
  if (!type || !type.is<code::TypeFunction>()) return;
  auto function = type.as<code::TypeFunction>();

  // Realize the function
  do_realize_function(x, *function, true);
}

void AnalyzeUsage::visit_argument(ast::Argument& x) {
  // Run the base method (analyze the argument)
  Visitor::visit_argument(x);
  if (Log::get().count("error") > 0) return;

  // Resolve the function-type of the operand
  auto type = Resolve(_scope).run(*x.expression);
  if (!type || !type.is<code::TypeFunction>()) return;
  auto function = type.as<code::TypeFunction>();

  // Realize the function
  do_realize_function(x, *function);
}

}  // namespace pass
}  // namespace arrow
