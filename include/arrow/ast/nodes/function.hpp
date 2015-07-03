// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_AST_NODES_FUNCTION_H
#define ARROW_AST_NODES_FUNCTION_H 1

#include "arrow/ast/nodes/node.hpp"

namespace arrow {
namespace ast {

/// An optionally named formal parameter of a function.
struct Parameter : Node {
  Parameter(
    Span span,
    Ref<Pattern> pattern,
    Ref<Type> type = nullptr,
    Ref<Node> default_value_ = nullptr
  ) : Node(span),
      pattern(pattern),
      type(type),
      default_value(default_value_) {
  }

  virtual ~Parameter() noexcept;

  virtual void accept(Visitor&);

  /// The (irrefutable) pattern.
  Ref<Pattern> pattern;

  /// The type annotation.
  Ref<Type> type;

  /// The (optional) default value.
  Ref<Node> default_value;
};

struct BaseFunction : Item {
  BaseFunction(
    Span span,
    bool exported,
    std::string name,
    Ref<Node> result_type_ = nullptr
  ) : Item(span, exported),
      name(name),
      result_type(result_type_) {
    if (result_type_ == nullptr) {
      // Give this function the `None` result
      // if it did not pass a result type
      result_type = new TypeNone(Span());
    }
  }

  virtual ~BaseFunction() noexcept;

  /// The name (from identifier).
  std::string name;

  /// The type annotation for the result.
  Ref<Node> result_type;

  /// The ordered sequence of parameters.
  std::deque<Ref<ast::Parameter>> parameters;
};

struct Function : BaseFunction {
  using BaseFunction::BaseFunction;

  virtual ~Function() noexcept;

  virtual void accept(Visitor&);

  Ref<Block> block;
};

struct ExternFunction : BaseFunction {
  ExternFunction(
    Span span,
    bool exported,
    std::string name,
    Ref<Node> result_type_ = nullptr,
    std::string abi_ = "cdcel"
  ) : BaseFunction(span, exported, name, result_type_),
      abi(abi_) {
  }

  virtual ~ExternFunction() noexcept;

  virtual void accept(Visitor&);

  /// The defined ABI (with the default being cdcel).
  std::string abi;
};

/// Return expression (divergent).
struct Return : Node {
  Return(Span span, Ref<Node> expression)
    : Node(span), expression(expression) {
  }

  virtual ~Return() noexcept;

  void accept(Visitor&) override;

  Ref<Node> expression;
};

/// An anonymous function expression.
struct Lambda : Node {
  Lambda(
    Span span,
    Ref<Node> result_type_
  ) : Node(span),
      result_type(result_type_) {
  }

  virtual ~Lambda() noexcept;

  /// The type annotation for the result.
  Ref<Node> result_type;

  /// The ordered sequence of parameters.
  /// NOTE: That because this is an anonymous function expression; the types
  ///       of function parameters are expected to be `null`.
  std::deque<Ref<ast::Parameter>> parameters;
};

}  // namespace ast
}  // namespace arrow

#endif  // ARROW_AST_NODES_FUNCTION_H
