// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_AST_NODES_PATTERN_H
#define ARROW_AST_NODES_PATTERN_H 1

#include "arrow/ast/nodes/node.hpp"
#include "arrow/ast/nodes/literal.hpp"

namespace arrow {
namespace ast {

struct Pattern : Node {
  using Node::Node;

  virtual ~Pattern() noexcept;
};

// Represents a wildcard pattern; `_`
struct PatternWildcard : Pattern {
  using Pattern::Pattern;

  virtual ~PatternWildcard() noexcept;

  void accept(Visitor&) override;
};

// Represents a singluar identifier pattern (that would bind
// to the match expression)
struct PatternIdentifier : Pattern {
  PatternIdentifier(Span span, std::string text, bool is_mutable)
    : Pattern(span), text(text), is_mutable(is_mutable) {
  }

  virtual ~PatternIdentifier() noexcept;

  void accept(Visitor&) override;

  std::string text;

  bool is_mutable;
};

// Represents a literal pattern (think equality check)
struct PatternLiteral : Pattern {
  PatternLiteral(Span span, Ref<Literal> literal)
    : Pattern(span), literal(literal) {
  }

  virtual ~PatternLiteral() noexcept;

  void accept(Visitor&) override;

  Ref<Literal> literal;
};

// Represents a tuple pattern (that would unpack a tuple and
// match on each element)
struct PatternTuple : Pattern {
  PatternTuple(Span span) : Pattern(span), elements() {
  }

  virtual ~PatternTuple() noexcept;

  void accept(Visitor&) override;

  std::deque<Ref<Pattern>> elements;
};

}  // namespace ast
}  // namespace arrow

#endif  // ARROW_AST_NODES_PATTERN_H
