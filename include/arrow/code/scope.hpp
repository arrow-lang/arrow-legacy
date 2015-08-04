// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_CODE_SCOPE_H
#define ARROW_CODE_SCOPE_H 1

#include <deque>
#include <map>
#include <unordered_map>

#include "arrow/ast/nodes/node.hpp"
#include "arrow/code/item.hpp"

namespace arrow {
namespace code {

class Scope;

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

  /// Get the parent block for this block.
  Ref<Block> parent() const { return _parent; }

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
  Ref<code::Item> find(const std::string& name, bool traverse = true);
  Ref<code::Item> find(
    ast::Node* context, bool traverse = true, bool unshadow = true);

 private:
  friend class Scope;

  /// Direct parent of this scope-block.
  Ref<Block> _parent;

  /// Items that have been added to this scope-block.
  std::deque<Ref<code::Item>> _items;

  /// Map of names->items (to be frequently overwritten to handle
  /// shadowing).
  std::unordered_map<std::string, unsigned> _items_by_name;

  /// Map of AST context->items (to be the canoncial method of looking
  /// up an item from a pass).
  std::unordered_map<ast::Node*, unsigned> _items_by_ctx;
};

class Container;

class Scope {
 public:
  explicit Scope(std::string name, Ref<Scope> parent = nullptr,
                 Container* owner = nullptr)
    : _name(name),
      _parent(parent),
      _blocks(),
      _stack(),
      _owner(owner) {
  }

  Scope(const Scope&) = delete;
  Scope(Scope&&) = delete;

  ~Scope() noexcept;

  Scope& operator=(const Scope&) = delete;
  Scope& operator=(Scope&&) = delete;

  /// Get the parent scope for this scope.
  Ref<Scope> parent() const { return _parent; }

  /// Grab the top-most scope-block.
  Ref<Block> top() {
    return _stack.size() > 0 ? _stack.front() : nullptr;
  }

  /// Enter a scope-block.
  Ref<Block> enter(ast::Node* context);

  /// Lookup a previously-entered scope-block.
  Ref<Block> at(ast::Node* context);

  /// Get the owner of this scope (module/function).
  Container* get_owner() const { return _owner; }

  /// Leave the last-entered scope-block.
  void exit();

  /// Insert an item in the current scope-block.
  void insert(Ref<Item> item);

  /// Check for an item in the current scope-block.
  bool contains(const std::string& name, bool traverse = true);
  bool contains(ast::Node* context, bool traverse = true);

  /// Attempt to get an item from the current scope-block.
  Ref<code::Item> find(const std::string& name, bool traverse = true);
  Ref<code::Item> find(
    ast::Node* context, bool traverse = true, bool unshadow = true,
    bool check_parent = true);

 private:
  /// Name of this scope.
  std::string _name;

  /// Direct (named) parent of this scope.
  Ref<Scope> _parent;

  /// Anonymous scope-blocks contained within this scope.
  std::map<ast::Node*, Ref<Block>> _blocks;

  /// Stack of active scope-blocks.
  std::deque<Ref<Block>> _stack;

  /// Owner of the scope (referential)
  /// FIXME: weak_ref
  Container* _owner;
};

}  // namespace code
}  // namespace arrow

#endif  // ARROW_CODE_SCOPE_H
