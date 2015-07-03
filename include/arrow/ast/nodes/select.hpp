// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_AST_NODES_SELECT_H
#define ARROW_AST_NODES_SELECT_H 1

#include "arrow/ast/nodes/node.hpp"

namespace arrow {
namespace ast {

struct Branch : Node {
  Branch(Span span, Ref<Node> condition, Ref<Block> block)
    : Node(span), block(block), condition(condition) {
  }

  virtual ~Branch() noexcept;

  void accept(Visitor&) override;

  Ref<Block> block;

  /// The conditional expression.
  Ref<Node> condition;
};

struct Select : Node {
  Select(Span span)
    : Node(span), else_block(), branches() {
  }

  virtual ~Select() noexcept;

  void accept(Visitor&) override;

  /// The `else` block for the selection; can be `null`.
  Ref<Block> else_block;

  /// The ordered sequence of branches.
  std::deque<Ref<Node>> branches;
};

}  // namespace ast
}  // namespace arrow

#endif  // ARROW_AST_NODES_SELECT_H
