// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/pass/analyze-usage.hpp"

namespace arrow {
namespace pass {

void AnalyzeUsage::visit_function(ast::Function& x) {
  // Pull out the previously-exposed item
  // TODO(mehcode): `scope->find<T>`
  auto item = _scope->find(&x).as<code::Function>();
  if (!item) return;

  // Create a child analyzer
  auto child = AnalyzeUsage(item->scope);

  // Analyze the function body
  child.run(*x.block);
}

}  // namespace pass
}  // namespace arrow
