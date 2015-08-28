// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/util.hpp"
#include "arrow/match.hpp"

namespace arrow {
namespace util {

Ref<code::Item> get_item(Ref<code::Scope>& _scope, ast::Node& node) {
  Match(node) {
    Case(ast::Identifier& ident) {
      auto item = _scope->find(ident.text);
      return item;
    } break;

    Case(ast::Path& path) {
      auto item = get_item(_scope, *path.operand);
      Match(*item) {
        Case(code::Import& imp) {
          auto& member = imp.module->items[path.member];
          return member;
        } break;
      } EndMatch;
    } break;
  } EndMatch;

  return nullptr;
}

Ref<code::Type> type_of(Ref<code::Item> item) {
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
  } else if (item.is<code::Parameter>()) {
    // This item -is- a function
    return item.as<code::Parameter>()->type;
  } else if (item.is<code::Structure>()) {
    // This item -is- a function
    return item.as<code::Structure>()->type;
  } else {
    // TODO(_): Error?
    return nullptr;
  }
}

bool is_mutable(code::Item& item) {
  Match(item) {
    Case(code::Slot& slot) {
      return slot.is_mutable;
    }

    Case(code::ExternSlot& slot) {
      return slot.is_mutable;
    }

    Case(code::Parameter& param) {
      return param.is_mutable;
    }
  } EndMatch;

  return false;
}

}  // namespace util
}  // namespace arrow
