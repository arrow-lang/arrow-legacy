// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/code/scope.hpp"

namespace arrow {
namespace code {

Scope::~Scope() noexcept {
}

void Scope::emplace(Ref<code::Item> item) {
  // Has this item already been added ..
  int index = -1;
  if (item->context) {
    auto ref = _items_by_ctx.find(item->context);
    if (ref != _items_by_ctx.end()) {
      // Yes; get the index and just re-activate in the name map
      index = ref->second;
    }
  }

  if (index == -1) {
    // No; push and map into both
    _items.push_back(item);
    index = (_items.size() - 1);
    _items_by_name[item->name] = index;
    _items_by_ctx[item->context] = index;
  } else {
    _items_by_name[item->name] = index;
  }
}

bool Scope::exists(ast::Node* context, bool traverse) const {
  return get(context, traverse) != nullptr;
}

bool Scope::exists(const std::string& name, bool traverse) const {
  return get(name, traverse) != nullptr;
}

auto Scope::get(ast::Node* context, bool traverse) const
    -> Ref<code::Item> {
  auto ref = _items_by_ctx.find(context);
  if (ref == _items_by_ctx.end()) {
    if (_parent && traverse) return _parent->get(context, traverse);
    return nullptr;
  }

  return _items.at(ref->second);
}

auto Scope::get(const std::string& name, bool traverse) const
    -> Ref<code::Item> {
  auto ref = _items_by_name.find(name);
  if (ref == _items_by_name.end()) {
    if (_parent && traverse) return _parent->get(name, traverse);
    return nullptr;
  }

  return _items.at(ref->second);
}

}  // namespace code
}  // namespace arrow
