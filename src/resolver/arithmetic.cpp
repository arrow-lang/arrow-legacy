// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/resolver.hpp"

using arrow::Resolver;
using arrow::resolve;

void Resolver::do_arithmetic(ast::Binary& x) {
  // Attempt to find a common type between the lhs and rhs
  // NOTE: We should validate that the operation is actually
  //  supported between the two types
  // NOTE: This needs to be extended once we support `ptr + int`
  auto type = common_type(x.lhs, x.rhs);
  if (!type) return;

  _stack.push(type);
}

void Resolver::visit(ast::Add& x) {
  do_arithmetic(x);
}

void Resolver::visit(ast::Sub& x) {
  do_arithmetic(x);
}

void Resolver::visit(ast::Mul& x) {
  do_arithmetic(x);
}

void Resolver::visit(ast::Div& x) {
  do_arithmetic(x);
}

void Resolver::visit(ast::Mod& x) {
  do_arithmetic(x);
}
