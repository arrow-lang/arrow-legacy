// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/resolver.hpp"
#include "arrow/log.hpp"

using arrow::Resolver;
using arrow::resolve;

void Resolver::visit_assign(ast::Assign& x) {
  // The resolution of an assignment is the RHS
  auto type = resolve(_g, _scope, *x.rhs);
  if (type) {
    _stack.push(type);
  }
}
