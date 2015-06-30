// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_AST_NODES_LITERAL_H
#define ARROW_AST_NODES_LITERAL_H 1

#include "arrow/ast/nodes/node.hpp"

namespace arrow {
namespace ast {
  
struct Identifier : TextNode {
  using TextNode::TextNode;

  virtual ~Identifier() noexcept;

  void accept(Visitor&) override;
};

/// A boolean literal (either `true` or `false`).
struct Boolean : Node {
  Boolean(Span span, bool value) : Node(span), value(value) {
  }

  virtual ~Boolean() noexcept;

  void accept(Visitor&) override;

  bool value;
};

/// An integer literal (`321`, `0xaffb`, '0B0100101', etc.).
struct Integer : TextNode {
  using TextNode::TextNode;

  virtual ~Integer() noexcept;

  void accept(Visitor&) override;
};

/// A float literal (`3.123`, `3e+10`, etc.)
struct Float : TextNode {
  using TextNode::TextNode;

  virtual ~Float() noexcept;

  void accept(Visitor&) override;
};

/// A string literal (`"Hello World"`).
struct String : TextNode {
  using TextNode::TextNode;

  virtual ~String() noexcept;

  void accept(Visitor&) override;
};

}  // namespace ast
}  // namespace arrow

#endif  // ARROW_AST_NODES_LITERAL_H
