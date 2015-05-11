// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/builder.hpp"
#include "arrow/resolver.hpp"
#include "arrow/generator.hpp"
#include "arrow/log.hpp"

using arrow::Builder;
using arrow::resolve;

void Builder::visit(ast::Add& x) {
  // Resolve the type
  auto type = resolve(_g, x);

  // Build the operands
  // TODO: Move some of this copy-pasta into helper methods
  // auto lhs = build_scalar<code::Value>(*(x.lhs))

  auto lhs_item = build_scalar(*(x.lhs));
  if (!lhs_item) return;
  if (!lhs_item->is_value()) {
    Log::get().error(x.lhs->span, "expected value");
    return;
  }
  auto rhs_item = build_scalar(*(x.rhs));
  if (!rhs_item) return;
  if (!rhs_item->is_value()) {
    Log::get().error(x.rhs->span, "expected value");
    return;
  }

  auto lhs = std::static_pointer_cast<code::Value>(lhs_item);
  auto rhs = std::static_pointer_cast<code::Value>(rhs_item);

  // Build the instruction
  auto handle = LLVMBuildAdd(_g._irb, lhs->handle(), rhs->handle(), "");

  // Build and push the code handle
  _stack.push(std::make_shared<code::Value>(handle, type));
}
