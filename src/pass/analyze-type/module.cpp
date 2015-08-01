// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/analyze-type.hpp"

namespace arrow {
namespace pass {

void AnalyzeType::visit_module(ast::Module& x) {
  // Get the existing module-item
  auto item = _ctx.modules[&x];
  if (!item) return;

  // Enter the module-scope block
  _scope->enter(&x);

  // Declare any items that need forward declarations.
  AnalyzeType(_ctx, item->scope).run(*x.block);

  // Leave the module scope-block
  _scope->exit();
}

}  // namespace pass
}  // namespace arrow