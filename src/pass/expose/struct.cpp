// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/expose.hpp"
#include "arrow/pass/type.hpp"

namespace arrow {
namespace pass {

void Expose::visit_struct(ast::Structure& x) {
  // Expose this into the current scope
  Ref<code::Item> item = new code::Structure(
    /*context=*/&x,
    /*name=*/x.name);

  // Create the initial opaque type record
  item.as<code::Structure>()->type = new code::TypeStructure(x.name);

  _scope->insert(item);

  // If exported; push into the module items
  if (x.exported) {
    auto mod = dynamic_cast<code::Module*>(_scope->get_owner());
    if (mod) {
      mod->items.emplace(x.name, item);
    }
  }
}

}  // namespace pass
}  // namespace arrow
