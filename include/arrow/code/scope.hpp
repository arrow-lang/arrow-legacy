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


// Assignments need to be hierarchial ..
/*

let a;          // <decl>
if cond {       // scope.enter / scope.enter
  a = 30;       // <assign>
} else {        // scope.exit / scope.enter
  if other {    // scope.enter / scope.enter
    a = 40;     // <assign>
  }             // scope.exit / scope.exit
}               // scope.exit / scope.exit

 */

class Block {
 public:
  explicit Block(Ref<Block> parent = nullptr)
    : _parent(parent),
      _items(),
      _items_by_name(),
      _items_by_ctx() {
  }

  Block(const Block&) = delete;
  Block(Block&&) = delete;

  ~Block() noexcept;

  Block& operator=(const Block&) = delete;
  Block& operator=(Block&&) = delete;

  /// Add an assignment to an item (in this specific scope-block).
  void add_assign(ast::Node* context, bool is_definite);

  /// Insert an item (to this specific scope-block).
  void insert(Ref<Item> item);

  /// Check for an item in the current scope-block.
  bool contains(const std::string& name, bool traverse = true);
  bool contains(ast::Node* context, bool traverse = true);

  /// Attempt to get an item from the current scope-block.
  ///
  /// @param traverse Indicate if we should recurse upwards to find a match
  ///                 if not available in the current scope-block.
  ///
  /// @param unshadow Indicate if we should unshadow a reference (if it was)
  ///                 even shadowed before).
  Ref<code::Item> get(const std::string& name, bool traverse = true);
  Ref<code::Item> get(
    ast::Node* context, bool traverse = true, bool unshadow = true);
};

class Scope {
 public:
  explicit Scope(Scope* parent = nullptr)
    : _current(nullptr), _parent(parent),
      _children(),
      _items(),
      _items_by_name(),
      _items_by_ctx() {
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

  void enter(ast::Node* context);
  void exit();

 private:
  /// Current scope reference.
  Scope* _current;

  /// Direct (named) parent of this scope.
  Scope* _parent;

  /// Anonymous scopes contained within this scope.
  std::unordered_map<ast::Node*, Ref<Scope>> _children;

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
