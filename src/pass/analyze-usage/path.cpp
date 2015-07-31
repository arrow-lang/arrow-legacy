// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

// #include "arrow/match.hpp"
#include "arrow/pass/analyze-usage.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void AnalyzeUsage::visit_path(ast::Path& x) {
  // Run the base method (analyze the operand and each argument)
  Visitor::visit_path(x);
  if (Log::get().count("error") > 0) return;
}

}  // namespace pass
}  // namespace arrow
