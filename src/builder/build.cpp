// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/builder.hpp"
#include "arrow/generator.hpp"
#include "arrow/log.hpp"

using arrow::Builder;
namespace code = arrow::code;
namespace ast = arrow::ast;

void Builder::build(ast::Node& node, code::Scope* scope) {
  // "Enter" the passed scope for this build operation
  auto os = _cs;
  if (scope != nullptr) {
    _cs = scope;
  }

  node.accept(*this);

  // Revert back to our previously "entered" scope
  _cs = os;
}

std::shared_ptr<code::Item> Builder::build_scalar(
    ast::Node& node, code::Scope* scope) {
  // Remember the size of the stack at this point (so we can
  // detect if an item gets pushed)
  auto cnt = _stack.size();

  build(node, scope);

  if ((_stack.size() - cnt) != 1) {
    // TODO: Report error (?)
    return nullptr;
  }

  auto item = _stack.top();
  _stack.pop();
  return item;
}
