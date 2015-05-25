// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <string>
#include "arrow/code/scope.hpp"

using arrow::code::Scope;

Scope::Scope() : _parent{nullptr}, _items{} {
}

Scope::Scope(Scope* parent) : _parent{parent}, _items{} {
}

auto Scope::operator=(const Scope& x) -> Scope& {
  _parent = x._parent;
  _items = x._items;

  return *this;
}

bool Scope::exists(const std::string& name, bool traverse) const {
  return get(name, traverse) != nullptr;
}

auto Scope::get(const std::string& name, bool traverse) const
    -> std::shared_ptr<Item> {
  auto ref = _items.find(name);
  if (ref != _items.end()) return ref->second;

  if (traverse && _parent != nullptr) return _parent->get(name);

  return nullptr;
}

void Scope::set(const std::string& name, std::shared_ptr<Item> item) {
  _items[name] = item;
}
