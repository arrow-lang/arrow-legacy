// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/util.hpp"
#include "arrow/pass/build.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void Build::visit_call(ast::Call& x) {
  // TODO(mehcode): Default
  // TODO(mehcode): Keyword

  // Build the functor operand ..
  auto op = Build(_ctx, _scope).run_scalar(*x.operand);
  if (!op) return;

  // Grab the type of the function ..
  // TODO(mehcode): Errors when this isn't a function type
  auto type = op->type.as<code::TypeFunction>();

  // Resolve and build each argument (if any)
  // TODO(mehcode): Check for mis-matched types
  // TODO(mehcode): Check for too many (or too little) arguments
  std::vector<LLVMValueRef> arguments;
  arguments.reserve(type->parameters.size());
  unsigned arg_index = 0;
  for (auto& arg_node : x.arguments) {
    // Build the argument expression
    auto arg = Build(_ctx, _scope).run_scalar(*arg_node->expression);
    if (!arg) return;

    // Cast the argument to the appropriate type
    arg = util::cast(
      _ctx, arg, *arg_node->expression,
      type->parameters.at(arg_index)->type, false);

    if (!arg) return;

    // Add it to the positional arguments
    arguments.push_back(arg->get_value(_ctx));
    arg_index += 1;
  }

  // Build the call IR
  auto res = LLVMBuildCall(
    _ctx.irb, op->get_value(_ctx), arguments.data(), arguments.size(), "");

  // TODO: Add sext/zext attributes if this function is of the C ABI

  // Push the resultant value
  _stack.push_front(new code::Value(res, type->result));
}

}  // namespace pass
}  // namespace arrow
