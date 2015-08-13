// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/code/value.hpp"

namespace arrow {
namespace code {

// Value (Base)
// -----------------------------------------------------------------------------

LLVMValueRef Value::get_address(Compiler::Context& ctx) {
  if (!has_address()) {
    // Capture an R-Value as an L-Value
    // TODO: We may want to make this behind a flag
    auto ptr = LLVMBuildAlloca(ctx.irb, type->handle(), "");
    LLVMBuildStore(ctx.irb, _handle, ptr);
    _handle = ptr;
  }

  return _handle;
}

LLVMValueRef Value::get_value(Compiler::Context& ctx) {
  if (has_address()) {
    return LLVMBuildLoad(ctx.irb, _handle, "");
  } else {
    return _handle;
  }
}

Ref<code::Value> Value::at(Compiler::Context& ctx, unsigned index) {
  if (!has_address()) return nullptr;

  // Ensure that `at` makes sense for the type
  Ref<code::Value> res = nullptr;
  Match(*type) {
    Case(const code::TypeTuple& x) {
      auto handle = LLVMBuildStructGEP(ctx.irb, get_address(ctx), index, "");
      res = new code::Value(handle, x.elements.at(index));
    } break;

    Otherwise() {
      return nullptr;
    }
  } EndMatch;

  return res;
}

// Tuple
// -----------------------------------------------------------------------------

bool ValueTuple::is_assignable() const {
  unsigned idx = 0;
  for (auto& el : elements) {
    if (!el->is_assignable()) return false;
    idx += 1;
  }

  return idx > 0;
}

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

Ref<code::Value> ValueTuple::at(Compiler::Context&, unsigned index) {
  return elements.at(index);
}

}  // namespace code
}  // namespace arrow
