// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <stdexcept>
#include "arrow/code/type.hpp"

namespace code = arrow::code;

#define IMPL(N) \
  code::N::~N() noexcept { }

IMPL(Type)
IMPL(IntegerType)
IMPL(FloatType)
IMPL(BooleanType)

code::IntegerType::IntegerType(unsigned bits, bool is_signed)
  : bits(bits), _is_signed(is_signed) {
}

code::FloatType::FloatType(unsigned bits)
  : bits(bits) {
}

LLVMTypeRef code::IntegerType::handle() const noexcept {
  return LLVMIntType(bits);
}

LLVMTypeRef code::BooleanType::handle() const noexcept {
  return LLVMInt1Type();
}

LLVMTypeRef code::FloatType::handle() const noexcept {
  switch (bits) {
    case 32:
      return LLVMFloatType();

    case 64:
      return LLVMDoubleType();

    default:
      throw std::runtime_error("invalid bit size for a floating-point type");
  }
}
