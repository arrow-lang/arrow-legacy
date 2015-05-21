// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/resolver.hpp"
#include "arrow/generator.hpp"
#include "arrow/log.hpp"

using arrow::Resolver;
namespace code = arrow::code;
namespace ast = arrow::ast;

Resolver::Resolver(arrow::Generator& g)
  : _g{g} {
}

Resolver::~Resolver() noexcept { }

void Resolver::visit(ast::Integer& x) {
  // By default, integer literals are as big as they need to be to fit
  //  the value (except they'll coerce upwards as needed)
  auto min = x.minimum_bits();
  auto bits = 0;

  if (min <= 8) {
    bits = 8;
  } else if (min <= 16) {
    bits = 16;
  } else if (min <= 32) {
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

  // By default, integer literals are signed
  _stack.push(std::make_shared<code::IntegerType>(bits, true));
}

void Resolver::visit(ast::Float&) {
  _stack.push(std::make_shared<code::FloatType>(64));
}

void Resolver::visit(ast::Boolean&) {
  _stack.push(std::make_shared<code::BooleanType>());
}

std::shared_ptr<code::Type> arrow::resolve(Generator& _g, ast::Node& x) {
  auto resolver = Resolver(_g);
  resolver.run(x);
  return resolver.get();
}

// Attempt to resolve a single compatible type from two passed
// types. Respects integer and float promotion rules.
std::shared_ptr<code::Type> Resolver::common_type(
  std::shared_ptr<code::Type> lhs,
  std::shared_ptr<code::Type> rhs
) {
  std::printf("common_type: begin\n");

  // If the types are the same; return the first
  // TODO: This needs to be extended into a full recursive comparison when
  //  we have generated types (eg. pointers of arbitrary depth)
  if (lhs == rhs) return lhs;

  if (lhs->is<code::IntegerType>() && rhs->is<code::IntegerType>()) {
    // We're dealing with two integer types; determine the integer
    // with the greater rank
    auto& int_lhs = lhs->as<code::IntegerType>();
    auto& int_rhs = rhs->as<code::IntegerType>();

    if (int_lhs.is_signed == int_rhs.is_signed) {
      // If the sign is equivalent; do a direct compare of bit size
      return (int_lhs.bits > int_rhs.bits) ? lhs : rhs;
    }

    if (int_lhs.is_signed && int_lhs.bits > int_rhs.bits) {
      return lhs;
    }

    if (int_rhs.is_signed && int_rhs.bits > int_lhs.bits) {
      return rhs;
    }
  }

  // Couldn't find a common type
  // TODO: Where should the error report be?
  std::printf("common_type: what are we?\n");
  return nullptr;
}
