// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/resolver.hpp"
#include "arrow/log.hpp"

using arrow::Resolver;
using arrow::resolve;

void Resolver::do_arithmetic(ast::Binary& x) {
  // Attempt to find a common type between the lhs and rhs
  // NOTE: This needs to be extended once we support `ptr + int`
  auto type = common_type(x.lhs, x.rhs);

  // Validate that the operation is actually supported between the two types
  // TODO: This obviously needs to change once we support overloading these
  if (!type || (!type->is<code::IntegerType>() &&
                !type->is<code::FloatType>()))
  {
    // Resolve the individual types
    auto lhs_type = resolve(_g, _scope, *x.lhs);
    auto rhs_type = resolve(_g, _scope, *x.rhs);
    if (!lhs_type || !rhs_type) return;

    // Report that we don't support arithmetic operators for you
    std::string op;
    if (x.is<ast::Add>()) {
      op = "+";
    } else if (x.is<ast::Sub>()) {
      op = "-";
    } else if (x.is<ast::Mul>()) {
      op = "*";
    } else if (x.is<ast::Div>()) {
      op = "/";
    } else if (x.is<ast::Mod>()) {
      op = "%";
    }

    Log::get().error(x.span,
      "cannot apply binary operator `%s` to types '%s' and '%s'",
      op.c_str(),
      lhs_type->name().c_str(),
      rhs_type->name().c_str());

    return;
  }

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
