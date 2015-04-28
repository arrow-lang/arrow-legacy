// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

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

bool Scope::exists(const std::string& name) const {
  return get(name) != nullptr;
}

auto Scope::get(const std::string& name) const -> std::shared_ptr<Item> {
  auto ref = _items.find(name);
  if (ref == _items.end()) return nullptr;

  if (_parent != nullptr) return _parent->get(name);

  return ref->second;
}

void Scope::set(const std::string& name, std::shared_ptr<Item> item) {
  _items[name] = item;
}
