// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/builder.hpp"
#include "arrow/resolver.hpp"
#include "arrow/generator.hpp"

using arrow::Builder;
using arrow::resolve;
namespace code = arrow::code;

void Builder::do_unary(
  ast::Unary& x,
  std::function<LLVMValueRef(std::shared_ptr<code::Value>)> cb
) {
  // Resolve the type
  auto type = resolve(_g, *_cs, x);
  if (!type) return;

  // Build the operand
  auto op = build_scalar_of<code::Value>(*x.operand);
  if (!op) return;

  // Perform the operation
  auto res = cb(op);

  // Build and push the code handle
  _stack.push(std::make_shared<code::Value>(&x, _cs, res, type));
}

// TODO: Rename the promotion operator to the identity operator?
void Builder::visit_promote(ast::Promote& x) {
  do_unary(x, [this](std::shared_ptr<code::Value> op) {
    // Promotion does nothing more than coerce into a rvalue
    return op->value_of(_g);
  });
}

void Builder::visit_negate_numeric(ast::NegateNumeric& x) {
  do_unary(x, [this](std::shared_ptr<code::Value> op) {
    if (op->type()->is<code::IntegerType>()) {
      return LLVMBuildNeg(_g._irb, op->value_of(_g), "");
    }

    if (op->type()->is<code::FloatType>()) {
      return LLVMBuildFNeg(_g._irb, op->value_of(_g), "");
    }

    return static_cast<LLVMOpaqueValue*>(nullptr);
  });
}

void Builder::visit_negate_logical(ast::NegateLogical& x) {
  do_unary(x, [this](std::shared_ptr<code::Value> op) {
    if (op->type()->is<code::BooleanType>()) {
      return LLVMBuildNot(_g._irb, op->value_of(_g), "");
    }

    return static_cast<LLVMOpaqueValue*>(nullptr);
  });
}

void Builder::visit_negate_bit(ast::NegateBit& x) {
  do_unary(x, [this](std::shared_ptr<code::Value> op) {
    if (op->type()->is<code::BooleanType>() ||
        op->type()->is<code::IntegerType>()) {
      return LLVMBuildNot(_g._irb, op->value_of(_g), "");
    }

    return static_cast<LLVMOpaqueValue*>(nullptr);
  });
}
