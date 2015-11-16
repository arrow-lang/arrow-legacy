// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/code/function.hpp"
#include "arrow/match.hpp"

namespace arrow {
namespace code {

// Extern Function
// -----------------------------------------------------------------------------

static LLVMAttribute _get_attribute(Ref<code::Type> type) {
  Match(*type) {
    Case(const code::TypeBoolean& x) {
      XTL_UNUSED(x);

      return LLVMZExtAttribute;
    } break;

    Case(const code::TypeInteger& x) {
      XTL_UNUSED(x);

      // TODO(mehcode): Won't be the case once this is arbitrary percision ..
      return LLVMSExtAttribute;
    } break;

    Case(const code::TypeSizedInteger& x) {
      // TODO(mehcode): Won't be the case once this is arbitrary percision ..
      return x.is_signed ? LLVMSExtAttribute : LLVMZExtAttribute;
    } break;
  } EndMatch;

  return static_cast<LLVMAttribute>(0);
}

LLVMValueRef ExternFunction::get_value(Compiler::Context& ctx) {
  if (!_handle) {
    // Try to get an existing function
    _handle = LLVMGetNamedFunction(ctx.mod, name.c_str());
    if (!_handle) {
      // Add the extern function decl to the module
      auto type_handle = LLVMGetElementType(type->handle());
      _handle = LLVMAddFunction(ctx.mod, name.c_str(), type_handle);

      // Check result type and apply attribute
      auto ftype = type.as<code::TypeFunction>();
      auto attr = _get_attribute(ftype->result);
      if (attr != 0) {
        LLVMAddResultAttribute(_handle, attr);
      }

      // Iterate through parameters, check, and apply attributes
      unsigned idx = 0;
      for (auto& param : ftype->parameters) {
        attr = _get_attribute(param->type);
        if (attr != 0) {
          LLVMAddAttribute(LLVMGetParam(_handle, idx), attr);
        }
        idx += 1;
      }
    }
  }

  return _handle;
}

}  // namespace code
}  // namespace arrow
