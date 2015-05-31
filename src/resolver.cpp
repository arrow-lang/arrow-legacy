// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <sstream>
#include "arrow/resolver.hpp"
#include "arrow/generator.hpp"
#include "arrow/log.hpp"

using arrow::Resolver;
namespace code = arrow::code;
namespace ast = arrow::ast;

Resolver::Resolver(arrow::Generator& g, code::Scope& scope)
  : _g{g}, _scope(scope) {
}

Resolver::~Resolver() noexcept { }

void Resolver::visit(ast::Integer& x) {
  // By default, integer literals are signed
  // By default, integer literals are as big as they need to be to fit
  //  the value (except they'll coerce upwards as needed)
  // NOTE: We add 1 for the sign bit
  auto min = x.minimum_bits() + 1;
  auto bits = 0;

  if (min <= 32) {
    bits = 32;
  } else if (min <= 64) {
    bits = 64;
  } else if (min <= 128) {
    bits = 128;
  } else {
    Log::get().error(
      x.span,
      "integer literal is too large to be represented in any integer type");

    return;
  }

  std::stringstream stream;
  stream << "int" << bits;
  _stack.push(std::static_pointer_cast<code::Type>(_scope.get(stream.str())));
}

void Resolver::visit(ast::Float&) {
  _stack.push(std::static_pointer_cast<code::Type>(_scope.get("float64")));
}

void Resolver::visit(ast::Boolean&) {
  _stack.push(std::static_pointer_cast<code::Type>(_scope.get("bool")));
}

std::shared_ptr<code::Type> arrow::resolve(
  Generator& g,
  code::Scope& scope,
  ast::Node& x
) {
  auto resolver = Resolver(g, scope);
  resolver.run(x);
  return resolver.get();
}

std::shared_ptr<code::Type> arrow::common_type(
  Generator& g,
  code::Scope& scope,
  std::shared_ptr<ast::Node> lhs,
  std::shared_ptr<ast::Node> rhs
) {
  auto resolver = Resolver(g, scope);
  return resolver.common_type(lhs, rhs);
}

// Attempt to resolve a single compatible type from two passed
// types. Respects integer and float promotion rules.
std::shared_ptr<code::Type> Resolver::common_type(
  std::shared_ptr<ast::Node> lhs,
  std::shared_ptr<ast::Node> rhs
) {
  // Resolve the operands
  auto lhs_ty = resolve(_g, _scope, *lhs);
  auto rhs_ty = resolve(_g, _scope, *rhs);

  // If the types are the same; return the first
  // TODO(mehcode): This needs to be extended into a full recursive
  //  comparison when
  //  we have generated types (eg. pointers of arbitrary depth)
  if (lhs_ty == rhs_ty) return lhs_ty;

  if (lhs_ty->is<code::IntegerType>() && rhs_ty->is<code::IntegerType>()) {
    // We're dealing with two integer types; determine the integer
    // with the greater rank
    auto& int_lhs = lhs_ty->as<code::IntegerType>();
    auto& int_rhs = rhs_ty->as<code::IntegerType>();

    if (int_lhs.is_signed() == int_rhs.is_signed()) {
      // If the sign is equivalent; do a direct compare of bit size
      return (int_lhs.bits > int_rhs.bits) ? lhs_ty : rhs_ty;
    }

    if (lhs->is<ast::Integer>() && int_lhs.bits <= int_rhs.bits) {
      // If we're dealing with a /literal/ -- just go with it as long as
      // we fit
      return rhs_ty;
    }

    if (rhs->is<ast::Integer>() && int_rhs.bits <= int_lhs.bits) {
      // If we're dealing with a /literal/ -- just go with it as long as
      // we fit
      return lhs_ty;
    }

    if (int_lhs.is_signed() && int_lhs.bits > int_rhs.bits) {
      return lhs_ty;
    }

    if (int_rhs.is_signed() && int_rhs.bits > int_lhs.bits) {
      return rhs_ty;
    }
  }

  if (lhs_ty->is<code::IntegerType>() && rhs_ty->is<code::FloatType>()) {
    return rhs_ty;
  }

  if (rhs_ty->is<code::IntegerType>() && lhs_ty->is<code::FloatType>()) {
    return lhs_ty;
  }

  if (rhs_ty->is<code::FloatType>() && lhs_ty->is<code::FloatType>()) {
    auto& float_lhs = lhs_ty->as<code::FloatType>();
    auto& float_rhs = rhs_ty->as<code::FloatType>();

    if (float_lhs.bits > float_rhs.bits) {
      return lhs_ty;
    } else {
      return rhs_ty;
    }
  }

  // Couldn't find a common type
  // TODO(mehcode): Where should the error report be?
  return nullptr;
}
