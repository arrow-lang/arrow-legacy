// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/build.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void Build::visit_array(ast::Array& x) {
  // Resolve the type of the literal
  auto type = Resolve(_scope).run(x);
  if (!type) return;

  // Segregate const and non-const elements
  // LLVM can create the initial array of const elements but subsequent
  // `insertelement` instructions must be done to add non-constant values.
  std::vector<LLVMValueRef> const_values;
  std::vector<std::pair<unsigned, LLVMValueRef>> non_const_values;

  unsigned idx = 0;
  for (auto& element : x.elements) {
    auto el_value = Build(_ctx, _scope).run_scalar(*element);
    if (!el_value) return;

    auto value = el_value->get_value(_ctx);
    if (LLVMIsConstant(value)) {
      const_values.push_back(value);
    } else {
      const_values.push_back(LLVMGetUndef(LLVMTypeOf(value)));
      non_const_values.push_back({idx, value});
    }
    idx += 1;
  }

  // Create the initial const
  auto type_a = type.as<code::TypeArray>();
  auto res = LLVMConstArray(
    type_a->element->handle(), const_values.data(), const_values.size());

  // Iterate through each non-const value
  for (auto& ncv : non_const_values) {
    res = LLVMBuildInsertValue(
      _ctx.irb, res, ncv.second, ncv.first, "");
  }

  _stack.push_front(new code::Value(res, type));
}

void Build::visit_index(ast::Index& x) {
  // Resolve the type of the expression
  auto type = Resolve(_scope).run(x);
  if (!type) return;

  // Build each operand ..
  auto lhs = Build(_ctx, _scope).run_scalar(*x.lhs);
  auto rhs = Build(_ctx, _scope).run_scalar(*x.rhs);
  if (!lhs || !rhs) return;

  // Build a GEP of the LHS to the RHS (index)
  auto index = rhs->get_value(_ctx);

  auto ptr = LLVMBuildBitCast(
    _ctx.irb, lhs->get_address(_ctx),
    LLVMPointerType(type->handle(), 0),
    "");

  auto res = LLVMBuildGEP(_ctx.irb, ptr, &index, 1, "");

  _stack.push_front(new code::Value(res, type));
}

}  // namespace pass
}  // namespace arrow
