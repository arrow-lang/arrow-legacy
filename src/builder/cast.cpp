// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/builder.hpp"
#include "arrow/resolver.hpp"

using arrow::Builder;
using arrow::resolve;

void Builder::visit_cast(ast::Cast& x) {
  // Build the operands
  auto lhs = build_scalar_of<code::Value>(*x.lhs);
  auto rhs = build_type(*x.rhs);
  if (!lhs || !rhs) return;

  // Perform the cast
  lhs = lhs->cast(_g, *x.lhs, rhs, true);
  if (!lhs) { return; }

  // Build and push the code handle
  _stack.push(std::make_shared<code::Value>(&x, _cs, lhs->value_of(_g), rhs));
}
