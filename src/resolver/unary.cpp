// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/resolver.hpp"
#include "arrow/log.hpp"

using arrow::Resolver;
using arrow::resolve;

void Resolver::visit(ast::Promote& x) {
  // Requires an integral or floating type
  auto type = resolve(_g, _scope, *x.operand);
  if (type->is<code::IntegerType>() || type->is<code::FloatType>()) {
    _stack.push(type);
    return;
  }

  Log::get().error(x.span,
    "cannot apply unary operator `+` to type '%s'",
    type->name().c_str());
}

void Resolver::visit(ast::NegateNumeric& x) {
  // Requires an integral or floating type
  auto type = resolve(_g, _scope, *x.operand);
  if (type->is<code::IntegerType>() || type->is<code::FloatType>()) {
    _stack.push(type);
    return;
  }

  Log::get().error(x.span,
    "cannot apply unary operator `-` to type '%s'",
    type->name().c_str());
}

void Resolver::visit(ast::NegateLogical& x) {
  // Requires a boolean type
  auto type = resolve(_g, _scope, *x.operand);
  if (type->is<code::BooleanType>()) {
    _stack.push(type);
    return;
  }

  Log::get().error(x.span,
    "cannot apply unary operator `not` to type '%s'",
    type->name().c_str());
}

void Resolver::visit(ast::NegateBit& x) {
  // Requires an integral or boolean type
  auto type = resolve(_g, _scope, *x.operand);
  if (type->is<code::BooleanType>() || type->is<code::IntegerType>()) {
    _stack.push(type);
    return;
  }

  Log::get().error(x.span,
    "cannot apply unary operator `!` to type '%s'",
    type->name().c_str());
}
