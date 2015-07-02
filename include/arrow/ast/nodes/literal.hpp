// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_AST_NODES_LITERAL_H
#define ARROW_AST_NODES_LITERAL_H 1

#include "arrow/ast/nodes/node.hpp"

namespace arrow {
namespace ast {

struct Literal : Node {
  using Node::Node;

  virtual ~Literal() noexcept;
};

// NOTE: Identifiers are not literals
struct Identifier : Node {
  Identifier(Span span, std::string text)
    : Node(span), text(text) {
  }

  virtual ~Identifier() noexcept;

  void accept(Visitor&) override;

  std::string text;
};

/// The `None` expression.
struct None : Literal {
  using Literal::Literal;

  virtual ~None() noexcept;

  void accept(Visitor&) override;
};

/// A boolean literal (either `true` or `false`).
struct Boolean : Literal {
  Boolean(Span span, bool value) : Literal(span), value(value) {
  }

  virtual ~Boolean() noexcept;

  void accept(Visitor&) override;

  bool value;
};

/// An integer literal (`321`, `0xaffb`, '0B0100101', etc.).
struct Integer : Literal {
  Integer(Span span, std::string text)
    : Literal(span), text(text) {
  }

  virtual ~Integer() noexcept;

  void accept(Visitor&) override;

  std::string text;
};

/// A float literal (`3.123`, `3e+10`, etc.)
struct Float : Literal {
  Float(Span span, std::string text)
    : Literal(span), text(text) {
  }

  virtual ~Float() noexcept;

  void accept(Visitor&) override;

  std::string text;
};

/// A string literal (`"Hello World"`).
struct String : Literal {
  String(Span span, std::string text)
    : Literal(span), text(text) {
  }

  virtual ~String() noexcept;

  void accept(Visitor&) override;

  std::string text;
};

}  // namespace ast
}  // namespace arrow

#endif  // ARROW_AST_NODES_LITERAL_H
