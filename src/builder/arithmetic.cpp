// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/builder.hpp"
#include "arrow/resolver.hpp"
#include "arrow/generator.hpp"

using arrow::Builder;
using arrow::resolve;
namespace code = arrow::code;

void Builder::do_arithmetic(
  ast::Binary& x,
  std::function<LLVMValueRef (std::shared_ptr<code::Value>, std::shared_ptr<code::Value>)> cb
) {
  // Resolve the type
  auto type = resolve(_g, *_cs, x);
  if (!type) return;

  // Build the operands
  auto lhs = build_scalar_of<code::Value>(*x.lhs);
  auto rhs = build_scalar_of<code::Value>(*x.rhs);
  if (!lhs || !rhs) return;

  // Perform appropriate casts (if needed)
  lhs = lhs->cast(_g, type);
  rhs = rhs->cast(_g, type);


  // Perform the operation
  auto res = cb(lhs, rhs);

  // Build and push the code handle
  _stack.push(std::make_shared<code::Value>(res, type));
}

void Builder::visit(ast::Add& x) {
  do_arithmetic(x, [this](std::shared_ptr<code::Value> lhs, std::shared_ptr<code::Value> rhs) {
    if (lhs->type()->is<code::IntegerType>()) {
      return LLVMBuildAdd(_g._irb, lhs->value_of(_g), rhs->value_of(_g), "");
    }

    if (lhs->type()->is<code::FloatType>()) {
      return LLVMBuildFAdd(_g._irb, lhs->value_of(_g), rhs->value_of(_g), "");
    }

    // TODO: Error ?
    return (LLVMOpaqueValue*)nullptr;
  });
}

void Builder::visit(ast::Sub& x) {
  do_arithmetic(x, [this](std::shared_ptr<code::Value> lhs, std::shared_ptr<code::Value> rhs) {
    if (lhs->type()->is<code::IntegerType>()) {
      return LLVMBuildSub(_g._irb, lhs->value_of(_g), rhs->value_of(_g), "");
    }

    if (lhs->type()->is<code::FloatType>()) {
      return LLVMBuildFSub(_g._irb, lhs->value_of(_g), rhs->value_of(_g), "");
    }

    // TODO: Error ?
    return (LLVMOpaqueValue*)nullptr;
  });
}

void Builder::visit(ast::Mul& x) {
  do_arithmetic(x, [this](std::shared_ptr<code::Value> lhs, std::shared_ptr<code::Value> rhs) {
    if (lhs->type()->is<code::IntegerType>()) {
      return LLVMBuildMul(_g._irb, lhs->value_of(_g), rhs->value_of(_g), "");
    }

    if (lhs->type()->is<code::FloatType>()) {
      return LLVMBuildFMul(_g._irb, lhs->value_of(_g), rhs->value_of(_g), "");
    }

    // TODO: Error ?
    return (LLVMOpaqueValue*)nullptr;
  });
}

void Builder::visit(ast::Div& x) {
  do_arithmetic(x, [this](std::shared_ptr<code::Value> lhs, std::shared_ptr<code::Value> rhs) {
    if (lhs->type()->is<code::IntegerType>()) {
      if (lhs->type()->is_signed()) {
        return LLVMBuildSDiv(_g._irb, lhs->value_of(_g), rhs->value_of(_g), "");
      } else {
        return LLVMBuildUDiv(_g._irb, lhs->value_of(_g), rhs->value_of(_g), "");
      }
    }

    if (lhs->type()->is<code::FloatType>()) {
      return LLVMBuildFDiv(_g._irb, lhs->value_of(_g), rhs->value_of(_g), "");
    }

    // TODO: Error ?
    return (LLVMOpaqueValue*)nullptr;
  });
}

void Builder::visit(ast::Mod& x) {
  do_arithmetic(x, [this](std::shared_ptr<code::Value> lhs, std::shared_ptr<code::Value> rhs) {
    if (lhs->type()->is<code::IntegerType>()) {
      if (lhs->type()->is_signed()) {
        return LLVMBuildSRem(_g._irb, lhs->value_of(_g), rhs->value_of(_g), "");
      } else {
        return LLVMBuildURem(_g._irb, lhs->value_of(_g), rhs->value_of(_g), "");
      }
    }

    if (lhs->type()->is<code::FloatType>()) {
      return LLVMBuildFRem(_g._irb, lhs->value_of(_g), rhs->value_of(_g), "");
    }

    // TODO: Error ?
    return (LLVMOpaqueValue*)nullptr;
  });
}
