// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_SCOPE_H
#define ARROW_SCOPE_H 1

#include <memory>
#include <string>
#include <unordered_map>

#include "arrow/code/item.hpp"

namespace arrow {
namespace code {

class Scope {
 public:
  Scope();
  explicit Scope(Scope* parent);

  Scope(const Scope& x);

  Scope& operator=(const Scope& x);

  /// Check if the passed name exists in this scope (or any parent scopes)
  bool exists(const std::string& name, bool traverse = true) const;

  /// Get the passed name (returns nullptr if it doesn't exist)
  std::shared_ptr<Item> get(const std::string& name, bool traverse = true) const;

  /// Set an item in this scope with the passed name
  void set(const std::string& name, std::shared_ptr<Item> item);

 private:
   Scope* _parent;
   std::unordered_map<std::string, std::shared_ptr<Item>> _items;

};

} // namespace code {
} // namespace arrow {

#endif // ARROW_SCOPE_H
