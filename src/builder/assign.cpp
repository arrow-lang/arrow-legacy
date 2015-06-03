// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/builder.hpp"
#include "arrow/resolver.hpp"
#include "arrow/generator.hpp"
#include "arrow/log.hpp"

using arrow::Builder;
using arrow::resolve;
namespace code = arrow::code;

void Builder::visit_assign(ast::Assign& x) {
  // Build the destination operand
  auto dst = build_scalar_of<code::Value>(*x.lhs);
  if (!dst) return;
  if (!dst->has_address()) {
    Log::get().error(x.span, "illegal left-hand side expression");
    return;
  }
  if (!dst->is_mutable()) {
    if (dst->is<code::Slot>()) {
      // FIXME: Need to do tracing to figure out if we are actually
      //  re-assigning
      auto& slot = dst->as<code::Slot>();
      Log::get().error(
        x.span, "(possible) re-assignment of immutable slot `%s`",
        slot.name.c_str());
    } else {
      Log::get().error(
        x.span, "re-assignment of immutable address");
    }

    return;
  }

  // Build the expression operand
  auto expr = build_scalar_of<code::Value>(*x.rhs);
  if (!expr) return;

  // Run an implicit cast to the destination
  expr = expr->cast(_g, *x.rhs, dst->type());
  if (!expr) return;

  // Build the store
  LLVMBuildStore(_g._irb, expr->value_of(_g), dst->address_of(_g));

  // Push the value of the expression
  _stack.push(expr);
}
