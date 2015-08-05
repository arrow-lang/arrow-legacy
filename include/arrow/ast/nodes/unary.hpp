// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_AST_NODES_UNARY_H
#define ARROW_AST_NODES_UNARY_H 1

#include "arrow/ast/nodes/node.hpp"

namespace arrow {
namespace ast {

/// A unary operation (`!x`, `*x`, `-x`, `+x`, `&x`).
struct Unary : Node {
  Unary(Span span, Ref<Node> operand) : Node(span), operand(operand) {
  }

  virtual ~Unary() noexcept;

  virtual const char* punctuator() const noexcept = 0;

  Ref<Node> operand;
};

#define DEF_UNARY(Name, P) \
  struct Name : Unary { \
    using Unary::Unary; \
    virtual ~Name() noexcept; \
    virtual const char* punctuator() const noexcept override { return P; } \
    void accept(Visitor&) override; \
  }

DEF_UNARY(Identity, "+");
DEF_UNARY(Negate, "-");
DEF_UNARY(BitNot, "!");
DEF_UNARY(Not, "not");
DEF_UNARY(Dereference, "*");

#undef DEF_UNARY

struct AddressOf : Unary {
  AddressOf(Span span, Ref<Node> operand, bool mut)
    : Unary(span, operand), is_mutable(mut) {
  }

  virtual ~AddressOf() noexcept;

  void accept(Visitor&) override;

  virtual const char* punctuator() const noexcept override {
    return "&";
  }

  bool is_mutable;
};

}  // namespace ast
}  // namespace arrow

#endif  // ARROW_AST_NODES_UNARY_H
