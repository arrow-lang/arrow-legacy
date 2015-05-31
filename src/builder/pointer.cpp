// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/builder.hpp"
#include "arrow/resolver.hpp"
#include "arrow/log.hpp"
#include "arrow/generator.hpp"

using arrow::Builder;
using arrow::resolve;

void Builder::visit(ast::PointerType& x) {
  // Attempt to resolve the type
  auto type = resolve(_g, _scope, x);
  if (!type) { return; }

  // Push the type
  _stack.push(type);
}

void Builder::visit(ast::Dereference& x) {
  do_unary(x, [this](std::shared_ptr<code::Value> op) {
    // Load our value
    return LLVMBuildLoad(_g._irb, op->value_of(_g), "");
  });
}

void Builder::visit(ast::AddressOf& x) {
  do_unary(x, [this](std::shared_ptr<code::Value> op) {
    // Get our address
    return op->address_of(_g);
  });
}
