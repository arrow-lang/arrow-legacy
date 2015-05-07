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

void Resolver::visit(ast::Float& x) {
  _stack.push(std::make_shared<code::FloatType>(64));
}

void Resolver::visit(ast::Boolean& x) {
  _stack.push(std::make_shared<code::BooleanType>());
}

std::shared_ptr<code::Type> arrow::resolve(Generator& _g, ast::Node& x) {
  auto resolver = Resolver(_g);
  resolver.run(x);
  return resolver.get();
}
