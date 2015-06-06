// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/resolver.hpp"
#include "arrow/log.hpp"

using arrow::Resolver;
using arrow::resolve;

void Resolver::visit_cast(ast::Cast& x) {
  // Resolve the type of the RHS
  auto rhs = resolve(_g, _scope, *x.rhs);
  if (!rhs) { return; }

  _stack.push(rhs);
}
