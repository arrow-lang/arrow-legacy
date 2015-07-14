// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_CODE_ITEM_H
#define ARROW_CODE_ITEM_H 1

#include "arrow/ast/nodes/node.hpp"
#include "arrow/llvm.hpp"

namespace arrow {
namespace code {

struct Item {
  Item(ast::Node* context, std::string name) : context(context), name(name) {
  }

  virtual ~Item() noexcept;

  /// AST node from whence this item came from (the source file).
  ast::Node* context;

  /// Name of the declaration item.
  std::string name;
};

}  // namespace code
}  // namespace arrow

#endif  // ARROW_CODE_ITEM_H
