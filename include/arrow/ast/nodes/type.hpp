// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_AST_NODES_TYPE_H
#define ARROW_AST_NODES_TYPE_H 1

#include <deque>

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
  explicit TypeTuple(Span span)
    : Type(span), elements() {
  }

  virtual ~TypeTuple() noexcept;

  void accept(Visitor&) override;

  // Elements of the tuple
  std::deque<Ref<Type>> elements;
};

struct TypeArray : Type {
  explicit TypeArray(Span span, Ref<Type> element, Ref<Node> size = nullptr)
    : Type(span), element(element), size(size) {
  }

  virtual ~TypeArray() noexcept;

  void accept(Visitor&) override;

  // Element of the array
  Ref<Type> element;

  // Size expression for the array
  Ref<Node> size;
};

struct TypeParameter : Type {
  TypeParameter(Span span, std::string keyword, Ref<Type> type, bool is_mutable)
    : Type(span), type(type), keyword(keyword), is_mutable(is_mutable) {
  }

  virtual ~TypeParameter() noexcept;

  void accept(Visitor&) override;

  Ref<Type> type;
  std::string keyword;
  bool is_mutable;
};

struct TypeFunction : Type {
  explicit TypeFunction(Span span)
    : Type(span), parameters(), result(nullptr) {
  }

  virtual ~TypeFunction() noexcept;

  void accept(Visitor&) override;

  std::deque<Ref<TypeParameter>> parameters;
  Ref<Type> result;
};

}  // namespace ast
}  // namespace arrow

#endif  // ARROW_AST_NODES_TYPE_H
