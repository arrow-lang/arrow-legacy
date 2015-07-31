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
  Ref<code::Item> item = new code::ExternFunction(
    /*context=*/&x,
    /*name=*/x.name,
    /*type=*/type);

  _scope->insert(item);

  // If exported; push into the module items
  if (x.exported) {
    auto mod = dynamic_cast<code::Module*>(_scope->get_owner());
    mod->items.emplace(x.name, item);
  }
}

}  // namespace pass
}  // namespace arrow
