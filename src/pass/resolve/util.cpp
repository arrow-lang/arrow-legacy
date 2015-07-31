// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

Ref<code::Type> Resolve::type_of(Ref<code::Item> item) {
  if (item.is<code::Slot>()) {
    // This item -is- a slot
    auto type = item.as<code::Slot>()->type;
    if (!type) {
      type = new code::TypeUnknown();
    }
    return type;
  } else if (item.is<code::ExternSlot>()) {
    // This item -is- an external slot
    return item.as<code::ExternSlot>()->type;
  } else if (item.is<code::ExternFunction>()) {
    // This item -is- an external function
    return item.as<code::ExternFunction>()->type;
  } else if (item.is<code::Function>()) {
    // This item -is- a function
    return item.as<code::Function>()->type;
  } else {
    // TODO(_): Error?
    return nullptr;
  }
}

}  // namespace pass
}  // namespace arrow
