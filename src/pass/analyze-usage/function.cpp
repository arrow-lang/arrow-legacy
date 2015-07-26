// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/pass/analyze-usage.hpp"

namespace arrow {
namespace pass {

void AnalyzeUsage::visit_function(ast::Function& x) {
  // Pull out the previously-exposed item
  auto item = _scope->find(&x).as<code::Function>();
  if (!item) return;

  // Check if we've analyzed the function (and exit early if we have)
  auto type = item->type.as<code::TypeFunction>();
  if (type->_is_analyzed) return;

  // Create a child analyzer
  auto child = AnalyzeUsage(item->scope);

  // Analyze the function body
  child.run(*x.block);

  // Push non-local assignments and uses onto the function type
  auto top = child._scope->at(x.block.get());
  for (auto& item : child._assign[top]) {
    if (!item->is_local(child._scope)) {
      bool is_assigned = *(item->is_assigned(top));
      type->_assign[item.get()] = is_assigned;
    }
  }

  // Push non-local uses onto the function type
  for (auto& item : child._use) {
    type->_use.insert(item);
  }

  // Mark analyzed
  type->_is_analyzed = true;
}

}  // namespace pass
}  // namespace arrow
