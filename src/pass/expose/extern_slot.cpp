// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/expose.hpp"
#include "arrow/pass/type.hpp"

namespace arrow {
namespace pass {

void Expose::visit_extern_slot(ast::ExternSlot& x) {
  // Build the type
  auto type = Type(_scope).run(*x.type);
  if (!type) return;

  // Expose this into the current scope
  Ref<code::Item> item = new code::ExternSlot(
    /*context=*/&x,
    /*name=*/x.name,
    /*type=*/type,
    /*is_mutable=*/x.is_mutable);

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
