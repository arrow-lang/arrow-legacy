// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/builder.hpp"
#include "arrow/generator.hpp"
#include "arrow/log.hpp"

using arrow::Builder;
namespace code = arrow::code;
namespace ast = arrow::ast;

void Builder::visit_return(ast::Return& x) {
  // Fail if we are not in a function
  if (!_cf) {
    // NOTE: This code is never executed (yet)
    Log::get().error(x.span, "`return` outside function");
    return;
  }

  // Build the return expression (if we have one)
  std::shared_ptr<code::Value> expr = nullptr;
  if (x.expression) {
    expr = build_scalar_of<code::Value>(*x.expression);
    if (!expr) { return; }
  }

  // Pull out the type of the current function
  auto type = _cf->type();

  if (expr) {
    // Perform appropriate casts (if needed)
    expr = expr->cast(_g, *x.expression, type->result);
    if (!expr) return;

    // Build the value return
    LLVMBuildRet(_g._irb, expr->value_of(_g));
  } else {
    if (type->result) {
      Log::get().error(x.span, "expected '%s' but found nothing",
        type->result->name().c_str());

      return;
    } else {
      // Unit result type; build the void return
      LLVMBuildRetVoid(_g._irb);
    }
  }
}
