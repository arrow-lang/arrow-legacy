// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/parser.hpp"
#include "arrow/pass/expose.hpp"
#include "arrow/pass/type.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void Expose::visit_module(ast::Module& x) {
  // Get the existing module-item
  auto item = _ctx.modules_by_context[&x];
  if (!item) return;

  // Expose the module block (into the new module scope).
  Expose(_ctx, item->scope).run(*x.block);
}

}  // namespace pass
}  // namespace arrow
