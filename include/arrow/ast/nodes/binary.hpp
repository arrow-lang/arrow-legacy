// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_AST_NODES_BINARY_H
#define ARROW_AST_NODES_BINARY_H 1

#include "arrow/ast/nodes/node.hpp"

namespace arrow {
namespace ast {

/// A binary operation (`x + y`, `y * x`).
struct Binary : Node {
  Binary(Span span, Ref<Node> lhs, Ref<Node> rhs)
    : Node(span), lhs(lhs), rhs(rhs) {
  }

  virtual ~Binary() noexcept;

  virtual const char* punctuator() const noexcept = 0;

  Ref<Node> lhs;
  Ref<Node> rhs;
};

#define DEF_BINARY(Name, P) \
  struct Name : Binary { \
    using Binary::Binary; \
    virtual ~Name() noexcept; \
    virtual const char* punctuator() const noexcept override { return P; } \
    void accept(Visitor&) override; \
  }

DEF_BINARY(Add, "+");
DEF_BINARY(Sub, "-");
DEF_BINARY(Mul, "*");
DEF_BINARY(Div, "/");
DEF_BINARY(Mod, "%");

DEF_BINARY(BitOr, "|");
DEF_BINARY(BitXor, "^");
DEF_BINARY(BitAnd, "&");

DEF_BINARY(EqualTo, "==");
DEF_BINARY(NotEqualTo, "!=");
DEF_BINARY(GreaterThanOrEqualTo, ">=");
DEF_BINARY(GreaterThan, ">");
DEF_BINARY(LessThanOrEqualTo, "<=");
DEF_BINARY(LessThan, "<");

DEF_BINARY(And, "and");
DEF_BINARY(Or, "or");

DEF_BINARY(Cast, "as");

DEF_BINARY(Assign, "=");

#undef DEF_BINARY

/// A member access operation (commonly referred to as the "dot" operator).
struct Path : Node {
  Path(Span span, Ref<Node> operand, std::string member)
    : Node(span), operand(operand), member(member) {
  }

  virtual ~Path() noexcept;

  void accept(Visitor&) override;

  Ref<Node> operand;
  std::string member;
};

}  // namespace ast
}  // namespace arrow

#endif  // ARROW_AST_NODES_BINARY_H
