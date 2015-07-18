// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void Resolve::visit_id(ast::Identifier& x) {
  auto item = _scope->get(x.text);
  if (item == nullptr) {
    Log::get().error(
      x.span, "use of unresolved name '%s'", x.text.c_str());

    return;
  }

  // Determine the type of said item
  if (item.is<code::Slot>()) {
    // This item -is- a slot
    auto type = item.as<code::Slot>()->type;
    if (!type) {
      type = new code::TypeUnknown();
    }
    _stack.push_front(type);
  } else if (item.is<code::ExternSlot>()) {
    // This item -is- an external slot
    _stack.push_front(item.as<code::ExternSlot>()->type);
  } else {
    // TODO: Error?
    return;
  }
}

}  // namespace pass
}  // namespace arrow
