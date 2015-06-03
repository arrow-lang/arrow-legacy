// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/resolver.hpp"
#include "arrow/log.hpp"

using arrow::Resolver;
using arrow::resolve;

void Resolver::do_bitwise(ast::Binary& x) {
  // Attempt to find a common type between the lhs and rhs
  auto type = common_type(x.lhs, x.rhs);

  // Validate that the operation is actually supported between the two types
  // TODO: This obviously needs to change once we support overloading these
  if (!type || (!type->is<code::IntegerType>() &&
                !type->is<code::BooleanType>()))
  {
    // Resolve the individual types
    auto lhs_type = resolve(_g, _scope, *x.lhs);
    auto rhs_type = resolve(_g, _scope, *x.rhs);
    if (!lhs_type || !rhs_type) return;

    // Report that we don't support arithmetic operators for you
    std::string op;
    if (x.is<ast::BitAnd>()) {
      op = "&";
    } else if (x.is<ast::BitOr>()) {
      op = "|";
    } else if (x.is<ast::BitXor>()) {
      op = "^";
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

void Resolver::visit_bit_and(ast::BitAnd& x) {
  do_bitwise(x);
}

void Resolver::visit_bit_xor(ast::BitXor& x) {
  do_bitwise(x);
}

void Resolver::visit_bit_or(ast::BitOr& x) {
  do_bitwise(x);
}
