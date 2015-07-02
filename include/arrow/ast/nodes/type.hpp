// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_AST_NODES_TYPE_H
#define ARROW_AST_NODES_TYPE_H 1

#include "arrow/ast/nodes/node.hpp"

namespace arrow {
namespace ast {

struct Type : Node {
  using Node::Node;

  virtual ~Type() noexcept;
};

struct TypeNone : Type {
  using Type::Type;

  virtual ~TypeNone() noexcept;

  void accept(Visitor&) override;
};

struct TypePath : Type {
  TypePath(Span span, std::vector<std::string> segments)
    : Type(span), segments(segments) {
  }

  virtual ~TypePath() noexcept;

  void accept(Visitor&) override;

  // Segments of the path (eg. `x.y.z` becomes [`x`, `y`, `z`])
  std::vector<std::string> segments;
};

struct TypePointer : Type {
  TypePointer(Span span, Ref<Type> pointee, bool is_mutable)
    : Type(span), pointee(pointee), is_mutable(is_mutable) {
  }

  virtual ~TypePointer() noexcept;

  void accept(Visitor&) override;

  // Pointee
  Ref<Type> pointee;

  // Expressed mutability of the pointee
  bool is_mutable;
};

struct TypeTuple : Type {
  TypeTuple(Span span)
    : Type(span), elements() {
  }

  virtual ~TypeTuple() noexcept;

  void accept(Visitor&) override;

  // Elements of the tuple
  std::deque<Ref<Type>> elements;
};

}  // namespace ast
}  // namespace arrow

#endif  // ARROW_AST_NODES_TYPE_H
