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
    Ref<Node> binding,
    Ref<Node> type = nullptr,
    Ref<Node> initializer = nullptr
  ) : Item(span, exported),
      binding(binding),
      type(type),
      initializer(initializer) {
  }

  /// The binding (identifier or tuple).
  Ref<Node> binding;

  /// The optional type annotation.
  Ref<Node> type;

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

  /// The name (in the external library).
  std::string name;

  /// The required (for external slots) type annotation.
  Ref<Node> type;
};

}  // namespace ast
}  // namespace arrow

#endif  // ARROW_AST_NODES_SLOT_H
