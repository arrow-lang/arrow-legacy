// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/code/scope.hpp"

namespace arrow {
namespace code {

// Block
// -----------------------------------------------------------------------------

Block::~Block() noexcept { }

void Block::insert(Ref<code::Item> item) {
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

bool Block::contains(ast::Node* context, bool traverse) {
  return find(context, traverse, false) != nullptr;
}

bool Block::contains(const std::string& name, bool traverse) {
  return find(name, traverse) != nullptr;
}

auto Block::find(ast::Node* context, bool traverse, bool unshadow)
    -> Ref<code::Item> {
  auto ref = _items_by_ctx.find(context);
  if (ref == _items_by_ctx.end()) {
    if (_parent && traverse) return _parent->find(context, traverse);
    return nullptr;
  }

  // Emplace the name
  // NOTE: Get by context is intended for visitors to explicitly
  //       request an item (and at the same time pushing that item
  //       to be unshadowed)
  auto item = _items.at(ref->second);
  if (unshadow) {
    _items_by_name[item->name] = ref->second;
  }

  return item;
}

auto Block::find(const std::string& name, bool traverse)
    -> Ref<code::Item> {
  auto ref = _items_by_name.find(name);
  if (ref == _items_by_name.end()) {
    if (_parent && traverse) return _parent->find(name, traverse);
    return nullptr;
  }

  return _items.at(ref->second);
}

// Scope (tree of blocks)
// -----------------------------------------------------------------------------

Scope::~Scope() noexcept {
}

Ref<code::Block> Scope::enter(ast::Node* context) {
  // Has this been entered before
  auto ref = _blocks.find(context);
  if (ref == _blocks.end()) {
    // Grab the top-most scope-block
    Ref<Block> top = _stack.size() > 0 ? _stack.front() : nullptr;

    // No; create a new scope-block
    _blocks.emplace(context, new Block(top));
  }

  // Push the block in the stack
  _stack.push_front(_blocks[context]);

  // Return the new block
  return _blocks[context];
}

void Scope::exit() {
  // Pop the last-entered scope-block
  _stack.pop_front();
}

void Scope::insert(Ref<code::Item> item) {
  // Proxy to the top-most scope-block
  _stack.front()->insert(item);
}

bool Scope::contains(ast::Node* context, bool traverse) {
  return find(context, traverse, false) != nullptr;
}

bool Scope::contains(const std::string& name, bool traverse) {
  return find(name, traverse) != nullptr;
}

Ref<code::Item> Scope::find(ast::Node* context, bool traverse, bool unshadow) {
  // Grab the top-most scope-block
  Ref<Block> top = _stack.size() > 0 ? _stack.front() : nullptr;

  // Proxy to the top-most scope-block
  auto item = top ? top->find(context, traverse, unshadow) : nullptr;

  // Check the parent (if we can and should)
  if (!item && _parent && traverse) {
    return _parent->find(context, traverse, unshadow);
  }

  // Just return what we found
  return item;
}

Ref<code::Item> Scope::find(const std::string& name, bool traverse) {
  // Grab the top-most scope-block
  Ref<Block> top = _stack.size() > 0 ? _stack.front() : nullptr;

  // Proxy to the top-most scope-block
  auto item = top ? top->find(name, traverse) : nullptr;

  // Check the parent (if we can and should)
  if (!item && _parent && traverse) {
    return _parent->find(name, traverse);
  }

  // Just return what we found
  return item;
}

}  // namespace code
}  // namespace arrow
