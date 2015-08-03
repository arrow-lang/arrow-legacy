// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/analyze-module.hpp"

namespace arrow {
namespace pass {

void AnalyzeModule::visit_function(ast::Function& x) {
  // Pull out the previously-exposed item
  auto item = _scope->find(&x).as<code::Function>();
  if (!item) return;

  // Check if we've analyzed the function (and exit early if we have)
  auto type = item->type.as<code::TypeFunction>();
  if (type->_is_module_analyzed) return;

  // Enter the function scope-block
  item->scope->enter(&x);

  // Create a child analyzer
  auto child = AnalyzeModule(_ctx, item->scope);

  // Mark us as in-the-middle of analyzing
  type->_is_module_analyzed = true;

  // Analyze the function body
  child.run(*x.block);

  // Exit the function scope-block
  item->scope->exit();
}

}  // namespace pass
}  // namespace arrow
