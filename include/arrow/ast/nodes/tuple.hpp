// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_AST_NODES_TUPLE_H
#define ARROW_AST_NODES_TUPLE_H 1

#include "arrow/ast/nodes/node.hpp"

namespace arrow {
namespace ast {

struct Tuple : Node {
  Tuple(Span span)
    : Node(span), elements() {
  }

  virtual ~Tuple() noexcept;

  void accept(Visitor&) override;

  // Elements of the tuple
  std::deque<Ref<Node>> elements;
};

}  // namespace ast
}  // namespace arrow

#endif  // ARROW_AST_NODES_TUPLE_H
