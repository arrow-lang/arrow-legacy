// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_AST_NODES_SLOT_H
#define ARROW_AST_NODES_SLOT_H 1

#include "arrow/ast/nodes/node.hpp"

namespace arrow {
namespace ast {

struct Slot : Item {
  Slot(
    Span span,
    bool exported,
    Ref<Pattern> pattern,
    Ref<Type> type = nullptr,
    Ref<Node> initializer = nullptr
  ) : Item(span, exported),
      pattern(pattern),
      type(type),
      initializer(initializer) {
  }

  virtual ~Slot() noexcept;

  void accept(Visitor&) override;

  /// The (irrefutable) pattern.
  Ref<Pattern> pattern;

  /// The optional type annotation.
  Ref<Type> type;

  /// The initializer expression for this slot.
  /// An immutable slot with an initializer is a constant.
  Ref<Node> initializer;
};

struct ExternSlot : Item {
  ExternSlot(
    Span span,
    bool exported,
    std::string name,
    Ref<Node> type
  ) : Item(span, exported),
      name(name),
      type(type) {
  }

  virtual ~ExternSlot() noexcept;

  void accept(Visitor&) override;

  /// The name (in the external library).
  std::string name;

  /// The required (for external slots) type annotation.
  Ref<Type> type;
};

}  // namespace ast
}  // namespace arrow

#endif  // ARROW_AST_NODES_SLOT_H
