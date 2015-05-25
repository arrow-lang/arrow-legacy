// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/builder.hpp"
#include "arrow/generator.hpp"
#include "arrow/expose.hpp"
#include "arrow/log.hpp"

using arrow::Builder;
namespace code = arrow::code;
namespace ast = arrow::ast;

void Builder::visit(ast::Return& x) {
  // Build the return expression (if we have one)
  std::shared_ptr<code::Value> expr = nullptr;
  if (x.expression) {
    expr = build_scalar_of<code::Value>(*x.expression);
    if (!expr) { return; }
  }

  // TODO(mehcode): Need to know what function I'm currently in so I
  //  know how to cast this
  if (expr) {
    LLVMBuildRet(_g._irb, expr->value_of(_g));
  } else {
    LLVMBuildRetVoid(_g._irb);
  }
}
