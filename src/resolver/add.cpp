// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/resolver.hpp"

using arrow::Resolver;
using arrow::resolve;

void Resolver::visit(ast::Add& x) {
  // Resolve the operands
  auto lhs = resolve(_g, *(x.lhs));
  auto rhs = resolve(_g, *(x.rhs));

  // Attempt to find a common type between the lhs and rhs
  // NOTE: We should validate that `+` is actually supported between the two
  //  types
  // NOTE: This needs to be extended once we support `ptr + int`
  std::printf("[Resolver::visit(ast::Add)] lhs: %p, rhs: %p\n", lhs.get(), rhs.get());

  auto type = common_type(lhs, rhs);
  if (!type) return;

  _stack.push(type);
}
