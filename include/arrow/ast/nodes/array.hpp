// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_AST_NODES_ARRAY_H
#define ARROW_AST_NODES_ARRAY_H 1

#include "arrow/ast/nodes/node.hpp"

namespace arrow {
namespace ast {

struct Array : Node {
  Array(Span span)
    : Node(span), elements() {
  }

  virtual ~Array() noexcept;

  void accept(Visitor&) override;

  // Elements of the array
  std::deque<Ref<Node>> elements;
};

}  // namespace ast
}  // namespace arrow

#endif  // ARROW_AST_NODES_ARRAY_H
