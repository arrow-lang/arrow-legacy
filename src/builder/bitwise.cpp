// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/builder.hpp"
#include "arrow/resolver.hpp"
#include "arrow/generator.hpp"

using arrow::Builder;
using arrow::resolve;
namespace code = arrow::code;

void Builder::do_bitwise(
  ast::Binary& x,
  std::function<LLVMValueRef(
    std::shared_ptr<code::Value>, std::shared_ptr<code::Value>)> cb
) {
  // Resolve the type
  auto type = resolve(_g, *_cs, x);
  if (!type) return;

  // Build the operands
  auto lhs = build_scalar_of<code::Value>(*x.lhs);
  auto rhs = build_scalar_of<code::Value>(*x.rhs);
  if (!lhs || !rhs) return;

  // Perform appropriate casts (if needed)
  lhs = lhs->cast(_g, *x.lhs, type);
  rhs = rhs->cast(_g, *x.rhs, type);
  if (!lhs || !rhs) return;

  // Perform the operation
  auto res = cb(lhs, rhs);

  // Build and push the code handle
  _stack.push(std::make_shared<code::Value>(&x, res, type));
}

void Builder::visit_bit_and(ast::BitAnd& x) {
  do_bitwise(x, [this](
    std::shared_ptr<code::Value> lhs, std::shared_ptr<code::Value> rhs
  ) {
    return LLVMBuildAnd(_g._irb, lhs->value_of(_g), rhs->value_of(_g), "");
  });
}

void Builder::visit_bit_or(ast::BitOr& x) {
  do_bitwise(x, [this](
    std::shared_ptr<code::Value> lhs, std::shared_ptr<code::Value> rhs
  ) {
    return LLVMBuildOr(_g._irb, lhs->value_of(_g), rhs->value_of(_g), "");
  });
}

void Builder::visit_bit_xor(ast::BitXor& x) {
  do_bitwise(x, [this](
    std::shared_ptr<code::Value> lhs, std::shared_ptr<code::Value> rhs
  ) {
    return LLVMBuildXor(_g._irb, lhs->value_of(_g), rhs->value_of(_g), "");
  });
}
