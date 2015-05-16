// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/resolver.hpp"

using arrow::Resolver;
using arrow::resolve;

void Resolver::visit(ast::Div& x) {
  // Resolve the operands
  auto lhs = resolve(_g, *(x.lhs));
  auto rhs = resolve(_g, *(x.rhs));

  // Pick the first one and push the code handle
  // TODO: Perform type promotion / checking
  _stack.push(lhs);
}
