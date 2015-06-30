// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_AST_NODES_LOOP_H
#define ARROW_AST_NODES_LOOP_H 1

#include "arrow/ast/nodes/node.hpp"

namespace arrow {
namespace ast {

struct Loop : Node {
  Loop(Span span, Ref<Block> block, Ref<Node> condition)
    : Node(span), block(block), condition(condition) {
  }

  virtual ~Loop() noexcept;

  void accept(Visitor&) override;

  Ref<Block> block;

  /// The conditional expression for the loop.
  /// If this is `null` then this is an infinite loop.
  Ref<Node> condition;
};

/// Break expression (divergent).
struct Break : Node {
  using Node::Node;

  virtual ~Break() noexcept;

  void accept(Visitor&) override;
};

/// Continue expression (divergent).
struct Continue : Node {
  using Node::Node;

  virtual ~Continue() noexcept;

  void accept(Visitor&) override;
};

}  // namespace ast
}  // namespace arrow

#endif  // ARROW_AST_NODES_LOOP_H
