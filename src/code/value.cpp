// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/code/value.hpp"

namespace arrow {
namespace code {

// Value (Base)
// -----------------------------------------------------------------------------

LLVMValueRef Value::get_address(Compiler::Context&) {
  if (!has_address()) return nullptr;
  return _handle;
}

LLVMValueRef Value::get_value(Compiler::Context& ctx) {
  if (has_address()) {
    return LLVMBuildLoad(ctx.irb, _handle, "");
  } else {
    return _handle;
  }
}

// Tuple
// -----------------------------------------------------------------------------

LLVMValueRef ValueTuple::get_value(Compiler::Context& ctx) {
  if (_handle) return _handle;

  // Segregate const and non-const elements
  // LLVM can create the initial tuple of const elements but subsequent
  // `insertvalue` instructions must be done to add non-constant values.
  std::vector<LLVMValueRef> const_values;
  std::vector<std::pair<unsigned, LLVMValueRef>> non_const_values;

  unsigned idx = 0;
  for (auto& element : elements) {
    auto value = element->get_value(ctx);
    if (LLVMIsConstant(value)) {
      const_values.push_back(value);
    } else {
      const_values.push_back(LLVMGetUndef(LLVMTypeOf(value)));
      non_const_values.push_back({idx, value});
    }
    idx += 1;
  }

  // Create the initial const struct
  _handle = LLVMConstStruct(const_values.data(), const_values.size(), false);

  // Iterate through each non-const value
  for (auto& ncv : non_const_values) {
    _handle = LLVMBuildInsertValue(
      ctx.irb, _handle, ncv.second, ncv.first, "");
  }

  return _handle;
}


}  // namespace code
}  // namespace arrow
