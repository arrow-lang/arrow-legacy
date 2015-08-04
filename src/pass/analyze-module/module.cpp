// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/analyze-module.hpp"

namespace arrow {
namespace pass {

void AnalyzeModule::visit_module(ast::Module& x) {
  // Get the existing module-item
  auto item = _ctx.modules_by_context[&x];
  if (!item) return;

  // Declare any items that need forward declarations.
  AnalyzeModule(_ctx, item->scope).run(*x.block);
}

}  // namespace pass
}  // namespace arrow
