// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/resolver.hpp"
#include "arrow/log.hpp"

using arrow::Resolver;
using arrow::resolve;

void Resolver::visit(ast::PointerType& x) {
  // Attempt to resolve the pointee
  auto pointee = resolve(_g, _scope, *x.pointee);
  if (!pointee) { return; }

  // Build and push the type
  _stack.push(std::make_shared<code::PointerType>(pointee));
}

void Resolver::visit(ast::AddressOf& x) {
  auto type = resolve(_g, _scope, *x.operand);
  if (!type->is<code::FunctionType>()) {
    // Build and push the type
    _stack.push(std::make_shared<code::PointerType>(type));

    return;
  }

  Log::get().error(x.span,
    "cannot apply unary operator `&` to type '%s'",
    type->name().c_str());
}

void Resolver::visit(ast::Dereference& x) {
  auto type = resolve(_g, _scope, *x.operand);
  if (type->is<code::PointerType>()) {
    auto& type_ptr = type->as<code::PointerType>();
    _stack.push(type_ptr.pointee);

    return;
  }

  Log::get().error(x.span,
    "cannot apply unary operator `*` to type '%s'",
    type->name().c_str());
}
