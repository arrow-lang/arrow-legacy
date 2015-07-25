// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/build.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void Build::visit_return(ast::Return& x) {
  // Check that we are /inside/ a function
  auto owner = _scope->get_owner();
  if (typeid(*owner) != typeid(code::Function)) {
    Log::get().error(x.span, "`return` outside of function");
    return;
  }

  // Get the function type (that we are returning from)
  auto type = dynamic_cast<code::Function*>(owner)->type;
  auto function = type.as<code::TypeFunction>();

  Ref<code::Value> op = nullptr;
  if (x.expression) {
    // Build the return expression ..
    op = Build(_ctx, _scope).run_scalar(*x.expression);
    if (!op) return;

    // Cast the expression to the appropriate type
    op = do_cast(op, *x.expression, function->result, false);
    if (!op) return;
  }

  // Build the return IR
  if (!function->result || function->result.is<code::TypeNone>()) {
    LLVMBuildRetVoid(_ctx.irb);
  } else {
    LLVMBuildRet(_ctx.irb, op->get_value(_ctx));
  }
}

}  // namespace pass
}  // namespace arrow
