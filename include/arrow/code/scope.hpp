// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_CODE_SCOPE_H
#define ARROW_CODE_SCOPE_H 1

#include <deque>
#include <unordered_map>

#include "arrow/ast/nodes/node.hpp"
#include "arrow/code/item.hpp"

namespace arrow {
namespace code {

class Scope {
 public:
  explicit Scope(Ref<Scope> parent = nullptr) : _parent(parent) {
  }

  Scope(const Scope&) = delete;
  Scope(Scope&&) = delete;

  ~Scope() noexcept;

  Scope& operator=(const Scope&) = delete;
  Scope& operator=(Scope&&) = delete;

  void emplace(Ref<code::Item> item);

  /// Attempt to get an item from the current scope.
  /// @param traverse Indicate if we should recurse upwards to find a match
  ///                 if not available in the current scope.
  Ref<code::Item> get(const std::string& name, bool traverse = true);
  Ref<code::Item> get(
    ast::Node* context, bool traverse = true, bool unshadow = true);

  bool exists(const std::string& name, bool traverse = true);
  bool exists(ast::Node* context, bool traverse = true);

 private:
  Ref<Scope> _parent;

  /// Items that have been added to this scope.
  std::deque<Ref<code::Item>> _items;

  /// Map of names->items (to be frequently overwritten to handle
  /// shadowing).
  std::unordered_map<std::string, unsigned> _items_by_name;

  /// Map of AST context->items (to be the canoncial method of looking
  /// up an item from a pass).
  std::unordered_map<ast::Node*, unsigned> _items_by_ctx;
};

}  // namespace code
}  // namespace arrow

#endif  // ARROW_CODE_SCOPE_H
