// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/code/value.hpp"

namespace arrow {
namespace code {

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

}  // namespace code
}  // namespace arrow
