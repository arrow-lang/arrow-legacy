// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/util.hpp"
#include "arrow/pass/build.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void Build::visit_call(ast::Call& x) {
  // Determine if we are a constructor (struct type) or a function
  if (x.operand.is<ast::Identifier>()) {
    auto id = x.operand.as<ast::Identifier>();
    auto item = _scope->find(id->text);
    if (item && item.is<code::Structure>()) {
      return do_struct_ctor(x, item);
    }
  }

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

void Build::do_struct_ctor(ast::Call& x, Ref<code::Structure> item) {
  auto& type = item->type;

  // Segregate const and non-const elements
  // LLVM can create the initial tuple of const elements but subsequent
  // `insertvalue` instructions must be done to add non-constant values.
  std::vector<LLVMValueRef> const_values;
  std::vector<std::pair<unsigned, LLVMValueRef>> non_const_values;

  unsigned idx = 0;
  for (auto& arg_node : x.arguments) {
    // Build the argument expression
    auto arg = Build(_ctx, _scope).run_scalar(*arg_node->expression);
    if (!arg) return;

    // Cast the argument to the appropriate type
    arg = util::cast(
      _ctx, arg, *arg_node->expression,
      type->members.at(idx)->type, false);

    if (!arg) return;

    auto value = arg->get_value(_ctx);
    if (LLVMIsConstant(value)) {
      const_values.push_back(value);
    } else {
      const_values.push_back(LLVMGetUndef(LLVMTypeOf(value)));
      non_const_values.push_back({idx, value});
    }

    idx += 1;
  }

  // Create the initial const struct
  auto handle =  LLVMConstNamedStruct(
    type->handle(), const_values.data(), const_values.size());

  // Iterate through each non-const value
  for (auto& ncv : non_const_values) {
    handle = LLVMBuildInsertValue(
      _ctx.irb, handle, ncv.second, ncv.first, "");
  }

  // Push the resultant value
  _stack.push_front(new code::Value(handle, type));
}

}  // namespace pass
}  // namespace arrow
