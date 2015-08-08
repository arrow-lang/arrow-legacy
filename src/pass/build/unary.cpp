// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <sstream>

#include "arrow/match.hpp"
#include "arrow/pass/resolve.hpp"
#include "arrow/pass/build.hpp"

namespace arrow {
namespace pass {

void Build::do_unary(
  ast::Unary& x, std::function<LLVMValueRef(Ref<code::Value>)> cb
) {
  // Resolve the type of the expression
  auto type = Resolve(_scope).run(x);
  if (!type) return;

  // Build the operand ..
  auto operand = Build(_ctx, _scope).run_scalar(*x.operand);
  if (!operand) return;

  // Perform the operation
  auto res = cb(operand);

  // Build and push the resultant value
  _stack.push_front(new code::Value(res, type));
}

void Build::visit_bit_not(ast::BitNot& x) {
  do_unary(x, [this](Ref<code::Value> operand) {
    LLVMValueRef res = nullptr;
    auto type = operand->type;
    if (type.is<code::TypeInteger>() ||
        type.is<code::TypeBoolean>() ||
        type.is<code::TypeSizedInteger>()) {
      res = LLVMBuildNot(_ctx.irb, operand->get_value(_ctx), "");
    }

    return res;
  });
}

void Build::visit_negate(ast::Negate& x) {
  do_unary(x, [this](Ref<code::Value> operand) {
    LLVMValueRef res = nullptr;
    auto type = operand->type;
    if (type.is<code::TypeInteger>() ||
        type.is<code::TypeSizedInteger>()) {
      res = LLVMBuildNeg(_ctx.irb, operand->get_value(_ctx), "");
    } else if (type.is<code::TypeFloat>()) {
      res = LLVMBuildFNeg(_ctx.irb, operand->get_value(_ctx), "");
    }

    return res;
  });
}

void Build::visit_not(ast::Not& x) {
  do_unary(x, [this](Ref<code::Value> operand) {
    LLVMValueRef res = nullptr;
    auto type = operand->type;
    if (type.is<code::TypeBoolean>()) {
      res = LLVMBuildNot(_ctx.irb, operand->get_value(_ctx), "");
    }

    return res;
  });
}

}  // namespace pass
}  // namespace arrow
