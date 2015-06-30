// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_AST_NODES_STRUCT_H
#define ARROW_AST_NODES_STRUCT_H 1

#include "arrow/ast/nodes/node.hpp"

namespace arrow {
namespace ast {

struct Member : Node {
  Member(
    Span span,
    std::string name_,
    Ref<Node> type_,
    Ref<Node> default_value_
  ) : Node(span),
      name(name_),
      type(type_),
      default_value(default_value_) {
  }

  virtual ~Member() noexcept;

  virtual void accept(Visitor&);

  /// The name (from identifier).
  std::string name;

  /// The type annotation.
  Ref<Node> type;

  /// The (optional) default value.
  Ref<Node> default_value;
};

struct Structure : Item {
  Structure(
    Span span,
    bool exported,
    std::string name
  ) : Item(span, exported), name(name), members() {
  }

  virtual ~Structure() noexcept;

  void accept(Visitor&) override;

  /// The name (from identifier).
  std::string name;

  /// The ordered sequence of members.
  std::deque<Ref<Member>> members;
};

}  // namespace ast
}  // namespace arrow

#endif  // ARROW_AST_NODES_STRUCT_H
