// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/code/type.hpp"

namespace arrow {
namespace code {

LLVMTypeRef BooleanType::handle() const {
  return LLVMInt1Type();
}

LLVMTypeRef FloatType::handle() const {
  return LLVMDoubleType();
}

LLVMTypeRef IntegerType::handle() const {
  return LLVMIntType(bits);
}

}  // namespace code
}  // namespace arrow
