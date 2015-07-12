// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_CODE_VALUE_H
#define ARROW_CODE_VALUE_H 1

#include "arrow/llvm.hpp"

namespace arrow {
namespace code {

struct Value {
  Value(LLVMValueRef handle, Ref<code::Type> type)
    : handle(handle), type(type) {
  }

  virtual ~Value() noexcept;

  LLVMValueRef handle;
  Ref<code::Type> type;
};

}  // namespace code
}  // namespace arrow

#endif  // ARROW_CODE_VALUE_H
