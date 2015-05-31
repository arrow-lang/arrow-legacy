// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/resolver.hpp"

using arrow::Resolver;
using arrow::resolve;

void Resolver::do_relational(ast::Binary& x) {
  // Attempt to find a common type between the lhs and rhs
  // NOTE: We should validate that the operation is actually
  //  supported between the two types
  auto type = common_type(x.lhs, x.rhs);
  if (!type) return;

  // NOTE: All relational operations return the `bool` type
  _stack.push(std::static_pointer_cast<code::Type>(_scope.get("bool")));
}

void Resolver::visit(ast::EqualTo& x) {
  do_relational(x);
}

void Resolver::visit(ast::NotEqualTo& x) {
  do_relational(x);
}

void Resolver::visit(ast::LessThan& x) {
  do_relational(x);
}

void Resolver::visit(ast::LessThanOrEqualTo& x) {
  do_relational(x);
}

void Resolver::visit(ast::GreaterThanOrEqualTo& x) {
  do_relational(x);
}

void Resolver::visit(ast::GreaterThan& x) {
  do_relational(x);
}
