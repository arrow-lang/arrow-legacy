// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <sstream>

#include "arrow/match.hpp"
#include "arrow/pass/resolve.hpp"
#include "arrow/pass/build.hpp"
#include "arrow/util.hpp"

namespace arrow {
namespace pass {

void Build::do_binary(
  ast::Binary& x,
  bool perform_cast,
  std::function<LLVMValueRef(
    Ref<code::Type>, Ref<code::Value>, Ref<code::Value>)> cb
) {
  // Resolve the type of the expression
  auto type = Resolve(_scope).run(x);
  if (!type) return;

  // Build each operand ..
  auto lhs = Build(_ctx, _scope).run_scalar(*x.lhs);
  auto rhs = Build(_ctx, _scope).run_scalar(*x.rhs);
  if (!lhs || !rhs) return;

  if (perform_cast) {
    // Cast the operands to the "common" type (not the resultant type)
    auto common_type = code::intersect_all({lhs->type, rhs->type});
    if (!common_type) return;

    lhs = util::cast(_ctx, lhs, *x.lhs, common_type, false);
    rhs = util::cast(_ctx, rhs, *x.rhs, common_type, false);
    if (!lhs || !rhs) return;
  }

  // Perform the operation
  auto res = cb(type, lhs, rhs);
  if (!res) return;

  // Build and push the resultant value
  _stack.push_front(new code::Value(res, type));
}

// Arithmetic
// -----------------------------------------------------------------------------

void Build::visit_add(ast::Add& x) {
  do_binary(x, true, [this](
    Ref<code::Type> type, Ref<code::Value> lhs, Ref<code::Value> rhs
  ) {
    LLVMValueRef res = nullptr;
    if (type.is<code::TypeInteger>() ||
        type.is<code::TypeIntegerLiteral>() ||
        type.is<code::TypeSizedInteger>()) {
      res = LLVMBuildAdd(
        _ctx.irb, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
    } else if (type.is<code::TypeFloat>()) {
      res = LLVMBuildFAdd(
        _ctx.irb, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
    }

    return res;
  });
}

void Build::visit_sub(ast::Sub& x) {
  do_binary(x, true, [this](
    Ref<code::Type> type, Ref<code::Value> lhs, Ref<code::Value> rhs
  ) {
    LLVMValueRef res = nullptr;
    if (type.is<code::TypeInteger>() ||
        type.is<code::TypeIntegerLiteral>() ||
        type.is<code::TypeSizedInteger>()) {
      res = LLVMBuildSub(
        _ctx.irb, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
    } else if (type.is<code::TypeFloat>()) {
      res = LLVMBuildFSub(
        _ctx.irb, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
    }

    return res;
  });
}

void Build::visit_mul(ast::Mul& x) {
  do_binary(x, true, [this](
    Ref<code::Type> type, Ref<code::Value> lhs, Ref<code::Value> rhs
  ) {
    LLVMValueRef res = nullptr;
    if (type.is<code::TypeInteger>() ||
        type.is<code::TypeIntegerLiteral>() ||
        type.is<code::TypeSizedInteger>()) {
      res = LLVMBuildMul(
        _ctx.irb, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
    } else if (type.is<code::TypeFloat>()) {
      res = LLVMBuildFMul(
        _ctx.irb, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
    }

    return res;
  });
}

void Build::visit_div(ast::Div& x) {
  do_binary(x, true, [this](
    Ref<code::Type> type, Ref<code::Value> lhs, Ref<code::Value> rhs
  ) {
    LLVMValueRef res = nullptr;
    if (type.is<code::TypeInteger>() || type.is<code::TypeIntegerLiteral>()) {
      res = LLVMBuildSDiv(
        _ctx.irb, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
    } else if (type.is<code::TypeSizedInteger>()) {
      if (type.as<code::TypeSizedInteger>()->is_signed) {
        res = LLVMBuildSDiv(
          _ctx.irb, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
      } else {
        res = LLVMBuildUDiv(
          _ctx.irb, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
      }
    } else if (type.is<code::TypeFloat>()) {
      res = LLVMBuildFDiv(
        _ctx.irb, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
    }

    return res;
  });
}

void Build::visit_mod(ast::Mod& x) {
  do_binary(x, true, [this](
    Ref<code::Type> type, Ref<code::Value> lhs, Ref<code::Value> rhs
  ) {
    LLVMValueRef res = nullptr;
    if (type.is<code::TypeInteger>() || type.is<code::TypeIntegerLiteral>()) {
      res = LLVMBuildSRem(
        _ctx.irb, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
    } else if (type.is<code::TypeSizedInteger>()) {
      if (type.as<code::TypeSizedInteger>()->is_signed) {
        res = LLVMBuildSRem(
          _ctx.irb, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
      } else {
        res = LLVMBuildURem(
          _ctx.irb, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
      }
    } else if (type.is<code::TypeFloat>()) {
      res = LLVMBuildFRem(
        _ctx.irb, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
    }

    return res;
  });
}

// Relational
// -----------------------------------------------------------------------------

void Build::visit_eq(ast::EqualTo& x) {
  do_binary(x, true, [this](
    Ref<code::Type>, Ref<code::Value> lhs, Ref<code::Value> rhs
  ) {
    LLVMValueRef res = nullptr;
    auto& type = lhs->type;

    if (type.is<code::TypeInteger>() ||
        type.is<code::TypeIntegerLiteral>() ||
        type.is<code::TypeBoolean>() ||
        type.is<code::TypeSizedInteger>()) {
      res = LLVMBuildICmp(
        _ctx.irb, LLVMIntEQ, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
    } else if (type.is<code::TypeFloat>()) {
      res = LLVMBuildFCmp(
        _ctx.irb, LLVMRealOEQ, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
    }

    return res;
  });
}

void Build::visit_ne(ast::NotEqualTo& x) {
  do_binary(x, true, [this](
    Ref<code::Type>, Ref<code::Value> lhs, Ref<code::Value> rhs
  ) {
    LLVMValueRef res = nullptr;
    auto& type = lhs->type;

    if (type.is<code::TypeInteger>() ||
        type.is<code::TypeIntegerLiteral>() ||
        type.is<code::TypeBoolean>() ||
        type.is<code::TypeSizedInteger>()) {
      res = LLVMBuildICmp(
        _ctx.irb, LLVMIntNE, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
    } else if (type.is<code::TypeFloat>()) {
      res = LLVMBuildFCmp(
        _ctx.irb, LLVMRealONE, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
    }

    return res;
  });
}

void Build::visit_lt(ast::LessThan& x) {
  do_binary(x, true, [this](
    Ref<code::Type>, Ref<code::Value> lhs, Ref<code::Value> rhs
  ) {
    LLVMValueRef res = nullptr;
    auto& type = lhs->type;

    if (type.is<code::TypeInteger>() ||
        type.is<code::TypeIntegerLiteral>()) {
      res = LLVMBuildICmp(
        _ctx.irb, LLVMIntSLT, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
    } else if (type.is<code::TypeBoolean>()) {
      res = LLVMBuildICmp(
        _ctx.irb, LLVMIntULT, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
    } else if (type.is<code::TypeSizedInteger>()) {
      if (type.as<code::TypeSizedInteger>()->is_signed) {
        res = LLVMBuildICmp(
          _ctx.irb, LLVMIntSLT, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
      } else {
        res = LLVMBuildICmp(
          _ctx.irb, LLVMIntULT, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
      }
    } else if (type.is<code::TypeFloat>()) {
      res = LLVMBuildFCmp(
        _ctx.irb, LLVMRealOLT, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
    }

    return res;
  });
}

void Build::visit_le(ast::LessThanOrEqualTo& x) {
  do_binary(x, true, [this](
    Ref<code::Type>, Ref<code::Value> lhs, Ref<code::Value> rhs
  ) {
    LLVMValueRef res = nullptr;
    auto& type = lhs->type;

    if (type.is<code::TypeInteger>() ||
        type.is<code::TypeIntegerLiteral>()) {
      res = LLVMBuildICmp(
        _ctx.irb, LLVMIntSLE, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
    } else if (type.is<code::TypeBoolean>()) {
      res = LLVMBuildICmp(
        _ctx.irb, LLVMIntULE, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
    } else if (type.is<code::TypeSizedInteger>()) {
      if (type.as<code::TypeSizedInteger>()->is_signed) {
        res = LLVMBuildICmp(
          _ctx.irb, LLVMIntSLE, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
      } else {
        res = LLVMBuildICmp(
          _ctx.irb, LLVMIntULE, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
      }
    } else if (type.is<code::TypeFloat>()) {
      res = LLVMBuildFCmp(
        _ctx.irb, LLVMRealOLE, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
    }

    return res;
  });
}

void Build::visit_gt(ast::GreaterThan& x) {
  do_binary(x, true, [this](
    Ref<code::Type>, Ref<code::Value> lhs, Ref<code::Value> rhs
  ) {
    LLVMValueRef res = nullptr;
    auto& type = lhs->type;

    if (type.is<code::TypeInteger>() ||
        type.is<code::TypeIntegerLiteral>()) {
      res = LLVMBuildICmp(
        _ctx.irb, LLVMIntSGT, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
    } else if (type.is<code::TypeBoolean>()) {
      res = LLVMBuildICmp(
        _ctx.irb, LLVMIntUGT, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
    } else if (type.is<code::TypeSizedInteger>()) {
      if (type.as<code::TypeSizedInteger>()->is_signed) {
        res = LLVMBuildICmp(
          _ctx.irb, LLVMIntSGT, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
      } else {
        res = LLVMBuildICmp(
          _ctx.irb, LLVMIntUGT, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
      }
    } else if (type.is<code::TypeFloat>()) {
      res = LLVMBuildFCmp(
        _ctx.irb, LLVMRealOGT, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
    }

    return res;
  });
}

void Build::visit_ge(ast::GreaterThanOrEqualTo& x) {
  do_binary(x, true, [this](
    Ref<code::Type>, Ref<code::Value> lhs, Ref<code::Value> rhs
  ) {
    LLVMValueRef res = nullptr;
    auto& type = lhs->type;

    if (type.is<code::TypeInteger>() ||
        type.is<code::TypeIntegerLiteral>()) {
      res = LLVMBuildICmp(
        _ctx.irb, LLVMIntSGE, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
    } else if (type.is<code::TypeBoolean>()) {
      res = LLVMBuildICmp(
        _ctx.irb, LLVMIntUGE, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
    } else if (type.is<code::TypeSizedInteger>()) {
      if (type.as<code::TypeSizedInteger>()->is_signed) {
        res = LLVMBuildICmp(
          _ctx.irb, LLVMIntSGE, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
      } else {
        res = LLVMBuildICmp(
          _ctx.irb, LLVMIntUGE, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
      }
    } else if (type.is<code::TypeFloat>()) {
      res = LLVMBuildFCmp(
        _ctx.irb, LLVMRealOGE, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
    }

    return res;
  });
}

// Bit-Wise
// -----------------------------------------------------------------------------

void Build::visit_bit_and(ast::BitAnd& x) {
  do_binary(x, true, [this](
    Ref<code::Type> type, Ref<code::Value> lhs, Ref<code::Value> rhs
  ) {
    LLVMValueRef res = nullptr;
    if (type.is<code::TypeInteger>() ||
        type.is<code::TypeIntegerLiteral>() ||
        type.is<code::TypeBoolean>() ||
        type.is<code::TypeSizedInteger>()) {
      res = LLVMBuildAnd(
        _ctx.irb, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
    }

    return res;
  });
}

void Build::visit_bit_or(ast::BitOr& x) {
  do_binary(x, true, [this](
    Ref<code::Type> type, Ref<code::Value> lhs, Ref<code::Value> rhs
  ) {
    LLVMValueRef res = nullptr;
    if (type.is<code::TypeInteger>() ||
        type.is<code::TypeIntegerLiteral>() ||
        type.is<code::TypeBoolean>() ||
        type.is<code::TypeSizedInteger>()) {
      res = LLVMBuildOr(
        _ctx.irb, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
    }

    return res;
  });
}

void Build::visit_bit_xor(ast::BitXor& x) {
  do_binary(x, true, [this](
    Ref<code::Type> type, Ref<code::Value> lhs, Ref<code::Value> rhs
  ) {
    LLVMValueRef res = nullptr;
    if (type.is<code::TypeInteger>() ||
        type.is<code::TypeIntegerLiteral>() ||
        type.is<code::TypeBoolean>() ||
        type.is<code::TypeSizedInteger>()) {
      res = LLVMBuildXor(
        _ctx.irb, lhs->get_value(_ctx), rhs->get_value(_ctx), "");
    }

    return res;
  });
}

}  // namespace pass
}  // namespace arrow
