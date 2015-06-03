// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/builder.hpp"
#include "arrow/resolver.hpp"
#include "arrow/log.hpp"
#include "arrow/generator.hpp"

using arrow::Builder;
using arrow::resolve;

void Builder::visit_pointer_type(ast::PointerType& x) {
  // Attempt to resolve the type
  auto type = resolve(_g, _scope, x);
  if (!type) { return; }

  // Push the type
  _stack.push(type);
}

void Builder::visit_dereference(ast::Dereference& x) {
  // Resolve the type
  auto type = resolve(_g, *_cs, x);
  if (!type) return;

  // Build the operand
  auto op = build_scalar_of<code::Value>(*x.operand);
  if (!op) return;

  // Perform the operation
  auto res = op->value_of(_g);
  if (!res) return;

  // Build and push the code handle
  _stack.push(std::make_shared<code::Value>(
    res, type, op->type()->is_mutable(), true));
}

void Builder::visit_address_of(ast::AddressOf& x) {
  // Resolve the type
  auto type = resolve(_g, *_cs, x);
  if (!type) return;

  // Build the operand
  auto op = build_scalar_of<code::Value>(*x.operand);
  if (!op) return;

  // Check that we can be mutable (if requested)
  if (x.is_mutable && !op->is_mutable()) {
    if (op->is<code::Slot>()) {
      auto name = op->as<code::Slot>().name;
      Log::get().error(x.span,
        "cannot capture immutable slot `%s` as mutable", name.c_str());
    } else {
      Log::get().error(x.span,
        "cannot capture immutable address as mutable");
    }

    return;
  }

  // Perform the operation
  auto res = op->address_of(_g);

  // Build and push the code handle
  _stack.push(std::make_shared<code::Value>(res, type));
}
