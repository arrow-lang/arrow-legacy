// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/analyze-type.hpp"

namespace arrow {
namespace pass {

void AnalyzeType::visit_module(ast::Module& x) {
  // Get the existing module-item
  auto item = _ctx.modules_by_context[&x];
  if (!item) return;

  // Ensure that we don't analyze a single module more than once on the
  // same pass-through
  if (_modules.find(item.get()) == _modules.end()) {
    _modules.insert(item.get());
  } else {
    return;
  }

  // Save the current-scope and set the item-scope
  auto current_scope = _scope;
  _scope = item->scope;

  // Analyze the module body
  x.block->accept(*this);

  // Restore the previous scope
  _scope = current_scope;
}

}  // namespace pass
}  // namespace arrow
