// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_AST_NODES_NODE_H
#define ARROW_AST_NODES_NODE_H 1

#include "arrow/ref.hpp"
#include "arrow/span.hpp"

namespace arrow {
namespace ast {

class Visitor;

/// Abstract base class of all AST nodes.
// TODO: Do something and make it illegal to instantiate these.
struct Node {
  explicit Node(Span span) : span(span) {
  }

  virtual ~Node() noexcept;

  virtual void accept(Visitor&);

  Span span;
};

struct Item : Node {
  Item(Span span, bool exported_)
    : Node(span), exported(exported_) {
  }

  virtual ~Item() noexcept;

  /// Whether this module-level item was exported and made
  /// available for import from other modules.
  bool exported;
};

}  // namespace ast
}  // namespace arrow

#endif  // ARROW_AST_NODES_NODE_H
