// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_CODE_ITEM_H
#define ARROW_CODE_ITEM_H 1

namespace arrow {

namespace ast {

struct Node;

}  // namespace ast

namespace code {

class Scope;

struct Item {
  Item(ast::Node* context, code::Scope* scope);

  virtual ~Item() noexcept;

  template <typename T>
  bool is() {
    return dynamic_cast<T*>(this) != nullptr;
  }

  template <typename T>
  T& as() {
    return *(dynamic_cast<T*>(this));
  }

  virtual bool is_type() const noexcept {
    return false;
  }

  virtual bool is_value() const noexcept {
    return false;
  }

  ast::Node* context;
  code::Scope* scope;
};

}  // namespace code
}  // namespace arrow

#endif  // ARROW_CODE_ITEM_H
