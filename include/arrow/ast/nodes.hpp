// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_AST_NODES_H
#define ARROW_AST_NODES_H 1

#include <deque>

#include "arrow/ast/nodes/node.hpp"
#include "arrow/ast/nodes/literal.hpp"

namespace arrow {
namespace ast {

/// A block of statements; its value (when asked)
/// is the value of its last statement.
struct Block : Node {
  Block(Span span) : Node(span) {}

  virtual ~Block() noexcept;

  virtual void accept(Visitor& v);

  /// The ordered sequence of statements.
  std::deque<Ref<Node>> statements;
};

/// Represents a logical, named module (could be a top-level module that
/// would be the file or a sub-module acting as a namespace).
struct Module : Node {
  Module(Span span, std::string name) : Node(span), name(name), block(nullptr) {
  }

  virtual ~Module() noexcept;

  void accept(Visitor&) override;

  std::string name;
  Ref<Block> block;
};

/// TypeOf expression.
struct TypeOf : Node {
  TypeOf(Span span, Ref<Node> expression)
    : Node(span), expression(expression) {
  }

  virtual ~TypeOf() noexcept;

  void accept(Visitor&) override;

  Ref<Node> expression;
};

}  // namespace ast
}  // namespace arrow

#include "arrow/ast/nodes/type.hpp"
#include "arrow/ast/nodes/pattern.hpp"
#include "arrow/ast/nodes/function.hpp"
#include "arrow/ast/nodes/loop.hpp"
#include "arrow/ast/nodes/unary.hpp"
#include "arrow/ast/nodes/binary.hpp"
#include "arrow/ast/nodes/call.hpp"
#include "arrow/ast/nodes/struct.hpp"
#include "arrow/ast/nodes/slot.hpp"
#include "arrow/ast/nodes/select.hpp"
#include "arrow/ast/nodes/tuple.hpp"
#include "arrow/ast/nodes/import.hpp"

#endif  // ARROW_AST_NODES_H
