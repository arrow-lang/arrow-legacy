// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/expose.hpp"
#include "arrow/pass/type.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void Expose::visit_extern_function(ast::ExternFunction& x) {
  // Resolve the type of this ..
  auto type = Resolve(_scope).run(x);
  if (!type) return;

  // Expose this into the current scope
  _scope->insert(new code::ExternFunction(
    /*context=*/&x,
    /*name=*/x.name,
    /*type=*/type));
}

}  // namespace pass
}  // namespace arrow
