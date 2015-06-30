// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_AST_NODES_CALL_H
#define ARROW_AST_NODES_CALL_H 1

#include "arrow/ast/nodes/node.hpp"

namespace arrow {
namespace ast {

/// An (optionally named) argument to a call expression.
struct Argument : Node {
  Argument(Span span, Ref<Node> expression, std::string name = "")
    : Node(span), expression(expression), name(name) {
  }

  virtual ~Argument() noexcept;

  void accept(Visitor&) override;

  Ref<Node> expression;

  /// The bound name for this argument; if "" then its a positional
  /// argument.
  std::string name;
};

/// A call (or function application) expression.
struct Call : Node {
  Call(Span span, Ref<Node> operand)
    : Node(span), operand(operand), arguments() {
  }

  virtual ~Call() noexcept;

  void accept(Visitor&) override;

  Ref<Node> operand;
  std::deque<Ref<ast::Argument>> arguments;
};

}  // namespace ast
}  // namespace arrow

#endif  // ARROW_AST_NODES_CALL_H
