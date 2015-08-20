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

}  // namespace pass
}  // namespace arrow
