// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_AST_NODES_IMPORT_H
#define ARROW_AST_NODES_IMPORT_H 1

#include "arrow/ast/nodes/node.hpp"

namespace arrow {
namespace ast {

struct Import : Node {
  Import(Span span, std::string name, std::string source)
    : Node(span), name(name), source(source) {
  }

  virtual ~Import() noexcept;

  void accept(Visitor&) override;

  std::string name;
  std::string source;
};

}  // namespace ast
}  // namespace arrow

#endif  // ARROW_AST_NODES_IMPORT_H
