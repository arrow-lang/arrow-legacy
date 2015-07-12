// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/build.hpp"
#include "arrow/pass/expose.hpp"
#include "arrow/pass/analyze.hpp"

namespace arrow {
namespace pass {

void Build::visit_module(ast::Module& x) {
  // Expose the module block (into the new module scope).
  Expose(_ctx, _scope).run(*x.block);

  // Analyze (usage analysis) the module block.
  Analyze(_scope).run(*x.block);

  // Visit the module block with the builder.
  x.block->accept(*this);
}

}  // namespace pass
}  // namespace arrow
