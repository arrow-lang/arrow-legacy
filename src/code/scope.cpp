// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <string>
#include <vector>
#include <sstream>
#include "arrow/code/scope.hpp"

using arrow::code::Scope;

Scope::Scope(const std::string& name) : _parent(nullptr), _name(name), _items() {
}

Scope::Scope(const std::string& name, Scope* parent) : _parent(parent), _name(name), _items() {
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

std::string Scope::name() const {
  std::stringstream stream;
  if (_parent) {
    auto parent_name = _parent->name();
    if (parent_name.size() != 0) {
      stream << parent_name;
      stream << ".";
    }
  }
  if (_name.size() != 0) {
    stream << _name;
  }
  return stream.str();
}
