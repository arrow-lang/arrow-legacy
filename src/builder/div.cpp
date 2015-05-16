// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/builder.hpp"
#include "arrow/resolver.hpp"
#include "arrow/generator.hpp"

using arrow::Builder;
using arrow::resolve;

void Builder::visit(ast::Div& x) {
  // Resolve the type
  auto type = resolve(_g, x);

  // Build the operands
  auto lhs = build_scalar_of<code::Value>(*x.lhs);
  auto rhs = build_scalar_of<code::Value>(*x.rhs);
  if (!lhs || !rhs) return;

  // Build the instruction
  auto handle = LLVMBuildSDiv(
    _g._irb, lhs->value_of(_g), rhs->value_of(_g), "");

  // Build and push the code handle
  _stack.push(std::make_shared<code::Value>(handle, type));
}
