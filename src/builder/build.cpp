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
    // TODO: Clear the stack up until our point
  }

  if (_stack.size() == 0) {
    return nullptr;
  }

  auto item = _stack.top();
  _stack.pop();
  return item;
}

std::shared_ptr<code::Type> Builder::build_type(
  ast::Node& node, code::Scope* scope
) {
  // Build the scalar `item`
  auto item = build_scalar(node, scope);
  if (!item) { return nullptr; }

  // Extract a type from this item
  if (item->is_type()) {
    // This /is/ a type -- return
    return std::dynamic_pointer_cast<code::Type>(item);
  } else if (item->is<code::Structure>()) {
    // Get the /type/ node of the structure
    return item->as<code::Structure>().type();
  }

  // Not a type
  Log::get().error(node.span, "expected a type expression");
  return nullptr;
}
