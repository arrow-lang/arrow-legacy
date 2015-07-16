// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/code/slot.hpp"

namespace arrow {
namespace code {

LLVMValueRef ExternSlot::handle(Compiler::Context& ctx) {
  if (!_handle) {
    _handle = LLVMAddGlobal(ctx.mod, type->handle(), name.c_str());
    LLVMSetExternallyInitialized(_handle, true);
  }

  return _handle;
}

LLVMValueRef ExternSlot::get_value(Compiler::Context& ctx) {
  return LLVMBuildLoad(ctx.irb, handle(ctx), "");
}

LLVMValueRef ExternSlot::get_address(Compiler::Context& ctx) {
  return handle(ctx);
}


}  // namespace code
}  // namespace arrow
