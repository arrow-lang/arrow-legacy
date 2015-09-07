// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_AST_NODES_ALIAS_H
#define ARROW_AST_NODES_ALIAS_H 1

#include "arrow/ast/nodes/node.hpp"
#include "arrow/ast/nodes/type.hpp"

namespace arrow {
namespace ast {

struct Alias : Item {
  Alias(
    Span span,
    bool exported,
    std::string name,
    Ref<Type> type
  ) : Item(span, exported), name(name), type(type) {
  }

  virtual ~Alias() noexcept;

  void accept(Visitor&) override;

  std::string name;

  Ref<Type> type;
};

}  // namespace ast
}  // namespace arrow

#endif  // ARROW_AST_NODES_ALIAS_H
