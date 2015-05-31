// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/builder.hpp"
#include "arrow/resolver.hpp"
#include "arrow/generator.hpp"

using arrow::Builder;
using arrow::resolve;
using arrow::common_type;
namespace code = arrow::code;

void Builder::do_relational(
  ast::Binary& x,
  std::function<int(std::shared_ptr<code::Type>)> cb
) {
  // Resolve the type; boolean
  auto type = resolve(_g, *_cs, x);
  if (!type) return;

  // Resolve the common type
  auto common = common_type(_g, *_cs, x.lhs, x.rhs);
  if (!common) return;

  // Build the operands
  auto lhs = build_scalar_of<code::Value>(*x.lhs);
  auto rhs = build_scalar_of<code::Value>(*x.rhs);
  if (!lhs || !rhs) return;

  // Perform appropriate casts (if needed)
  lhs = lhs->cast(_g, *x.lhs, common);
  rhs = rhs->cast(_g, *x.rhs, common);
  if (!lhs || !rhs) return;

  // Discover the operation
  auto op = cb(common);

  // Perform the operation
  LLVMValueRef res = nullptr;
  if (common->is<code::IntegerType>()) {
    res = LLVMBuildICmp(_g._irb, static_cast<LLVMIntPredicate>(op), lhs->value_of(_g), rhs->value_of(_g), "");
  }

  if (common->is<code::FloatType>()) {
    res = LLVMBuildFCmp(_g._irb, static_cast<LLVMRealPredicate>(op), lhs->value_of(_g), rhs->value_of(_g), "");
  }

  if (!res) {
    // TODO(mehcode): Error ?
    return;
  }

  // Build and push the code handle
  _stack.push(std::make_shared<code::Value>(res, type));
}

void Builder::visit(ast::EqualTo& x) {
  do_relational(x, [this](std::shared_ptr<code::Type> type) {
    if (type->is<code::IntegerType>()) {
      return static_cast<int>(LLVMIntEQ);
    }

    if (type->is<code::FloatType>()) {
      return static_cast<int>(LLVMRealOEQ);
    }

    return 0;
  });
}

void Builder::visit(ast::NotEqualTo& x) {
  do_relational(x, [this](std::shared_ptr<code::Type> type) {
    if (type->is<code::IntegerType>()) {
      return static_cast<int>(LLVMIntNE);
    }

    if (type->is<code::FloatType>()) {
      return static_cast<int>(LLVMRealONE);
    }

    return 0;
  });
}

void Builder::visit(ast::LessThan& x) {
  do_relational(x, [this](std::shared_ptr<code::Type> type) {
    if (type->is<code::IntegerType>()) {
      if (type->is_signed()) {
        return static_cast<int>(LLVMIntSLT);
      } else {
        return static_cast<int>(LLVMIntULT);
      }
    }

    if (type->is<code::FloatType>()) {
      return static_cast<int>(LLVMRealOLT);
    }

    return 0;
  });
}

void Builder::visit(ast::LessThanOrEqualTo& x) {
  do_relational(x, [this](std::shared_ptr<code::Type> type) {
    if (type->is<code::IntegerType>()) {
      if (type->is_signed()) {
        return static_cast<int>(LLVMIntSLE);
      } else {
        return static_cast<int>(LLVMIntULE);
      }
    }

    if (type->is<code::FloatType>()) {
      return static_cast<int>(LLVMRealOLE);
    }

    return 0;
  });
}

void Builder::visit(ast::GreaterThanOrEqualTo& x) {
  do_relational(x, [this](std::shared_ptr<code::Type> type) {
    if (type->is<code::IntegerType>()) {
      if (type->is_signed()) {
        return static_cast<int>(LLVMIntSGE);
      } else {
        return static_cast<int>(LLVMIntUGE);
      }
    }

    if (type->is<code::FloatType>()) {
      return static_cast<int>(LLVMRealOGE);
    }

    return 0;
  });
}

void Builder::visit(ast::GreaterThan& x) {
  do_relational(x, [this](std::shared_ptr<code::Type> type) {
    if (type->is<code::IntegerType>()) {
      if (type->is_signed()) {
        return static_cast<int>(LLVMIntSGT);
      } else {
        return static_cast<int>(LLVMIntUGT);
      }
    }

    if (type->is<code::FloatType>()) {
      return static_cast<int>(LLVMRealOGT);
    }

    return 0;
  });
}
