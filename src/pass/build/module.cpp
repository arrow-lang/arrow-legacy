// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/build.hpp"
#include "arrow/pass/expose.hpp"

namespace arrow {
namespace pass {

void Build::visit_module(ast::Module& x) {
  std::printf("Build::visit_module\n");

  // Expose the module block (into the new module scope)
  Expose(_ctx, _scope).run(*x.block);
}

}  // namespace pass
}  // namespace arrow
