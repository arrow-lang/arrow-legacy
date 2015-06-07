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
  std::function<LLVMValueRef(
    std::shared_ptr<code::Value>, std::shared_ptr<code::Value>)> cb
) {
  // Resolve the types of the operands
  auto lhs_type = resolve(_g, *_cs, *x.lhs);
  auto rhs_type = resolve(_g, *_cs, *x.rhs);
  if (!lhs_type || !rhs_type) return;

  // Resolve the target type
  auto type = resolve(_g, *_cs, x);
  if (!type) return;

  // Build the operands
  auto lhs = build_scalar_of<code::Value>(*x.lhs);
  auto rhs = build_scalar_of<code::Value>(*x.rhs);
  if (!lhs || !rhs) return;

  if (!(lhs_type->is<code::PointerType>() ||
        rhs_type->is<code::PointerType>())) {
    // Perform appropriate casts (if needed)
    lhs = lhs->cast(_g, *x.lhs, type);
    rhs = rhs->cast(_g, *x.rhs, type);
    if (!lhs || !rhs) return;
  }

  // Perform the operation
  auto res = cb(lhs, rhs);

  // Build and push the code handle
  _stack.push(std::make_shared<code::Value>(&x, res, type));
}

void Builder::visit_add(ast::Add& x) {
  do_arithmetic(x, [this](
    std::shared_ptr<code::Value> lhs, std::shared_ptr<code::Value> rhs
  ) {
    if (lhs->type()->is<code::PointerType>() &&
        rhs->type()->is<code::IntegerType>()) {
      // PTR + INT
      auto rhs_val = rhs->value_of(_g);
      return LLVMBuildGEP(
        _g._irb, lhs->value_of(_g), &rhs_val, 1, "");
    }

    if (rhs->type()->is<code::PointerType>() &&
        lhs->type()->is<code::IntegerType>()) {
      // INT + PTR (communative with PTR + INT)
      auto lhs_val = lhs->value_of(_g);
      return LLVMBuildGEP(
        _g._irb, rhs->value_of(_g), &lhs_val, 1, "");
    }

    if (lhs->type()->is<code::IntegerType>()) {
      return LLVMBuildAdd(_g._irb, lhs->value_of(_g), rhs->value_of(_g), "");
    }

    if (lhs->type()->is<code::FloatType>()) {
      return LLVMBuildFAdd(_g._irb, lhs->value_of(_g), rhs->value_of(_g), "");
    }

    // TODO(mehcode): Error ?
    return static_cast<LLVMOpaqueValue*>(nullptr);
  });
}

void Builder::visit_sub(ast::Sub& x) {
  do_arithmetic(x, [this](
    std::shared_ptr<code::Value> lhs, std::shared_ptr<code::Value> rhs
  ) {
    if (lhs->type()->is<code::PointerType>() &&
        rhs->type()->is<code::IntegerType>()) {
      // PTR - INT
      auto rhs_val = LLVMBuildNeg(_g._irb, rhs->value_of(_g), "");
      return LLVMBuildGEP(
        _g._irb, lhs->value_of(_g), &rhs_val, 1, "");
    }

    if (lhs->type()->is<code::PointerType>() &&
        rhs->type()->is<code::PointerType>()) {
      // PTR - PTR > Difference

      // Get our pointer type
      auto ptrtype = LLVMIntPtrType(_g._data_layout);

      // Convert both pointers to integers
      auto lhs_val = LLVMBuildPtrToInt(_g._irb, lhs->value_of(_g), ptrtype, "");
      auto rhs_val = LLVMBuildPtrToInt(_g._irb, rhs->value_of(_g), ptrtype, "");

      // Build a SUB instruction
      auto res = LLVMBuildSub(_g._irb, lhs_val, rhs_val, "");

      // Perform an exact [SU]DIV to determine the size
      // return res;
      auto pointee_type = lhs->type()->as<code::PointerType>().pointee->handle();
      return LLVMBuildExactSDiv(
        _g._irb, res, LLVMSizeOf(pointee_type), "");
    }

    if (lhs->type()->is<code::IntegerType>()) {
      return LLVMBuildSub(_g._irb, lhs->value_of(_g), rhs->value_of(_g), "");
    }

    if (lhs->type()->is<code::FloatType>()) {
      return LLVMBuildFSub(_g._irb, lhs->value_of(_g), rhs->value_of(_g), "");
    }

    // TODO(mehcode): Error ?
    return static_cast<LLVMOpaqueValue*>(nullptr);
  });
}

void Builder::visit_mul(ast::Mul& x) {
  do_arithmetic(x, [this](
    std::shared_ptr<code::Value> lhs, std::shared_ptr<code::Value> rhs
  ) {
    if (lhs->type()->is<code::IntegerType>()) {
      return LLVMBuildMul(_g._irb, lhs->value_of(_g), rhs->value_of(_g), "");
    }

    if (lhs->type()->is<code::FloatType>()) {
      return LLVMBuildFMul(_g._irb, lhs->value_of(_g), rhs->value_of(_g), "");
    }

    // TODO(mehcode): Error ?
    return static_cast<LLVMOpaqueValue*>(nullptr);
  });
}

void Builder::visit_div(ast::Div& x) {
  do_arithmetic(x, [this](
    std::shared_ptr<code::Value> lhs, std::shared_ptr<code::Value> rhs
  ) {
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

    // TODO(mehcode): Error ?
    return static_cast<LLVMOpaqueValue*>(nullptr);
  });
}

void Builder::visit_mod(ast::Mod& x) {
  do_arithmetic(x, [this](
    std::shared_ptr<code::Value> lhs, std::shared_ptr<code::Value> rhs
  ) {
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

    // TODO(mehcode): Error ?
    return static_cast<LLVMOpaqueValue*>(nullptr);
  });
}
