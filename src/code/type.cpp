// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <stdexcept>
#include <vector>
#include "arrow/code/type.hpp"

namespace code = arrow::code;

#define IMPL(N) \
  code::N::~N() noexcept { }

IMPL(Type)
IMPL(IntegerType)
IMPL(StringType)
IMPL(FloatType)
IMPL(BooleanType)
IMPL(FunctionType)

code::IntegerType::IntegerType(unsigned bits, bool is_signed)
  : bits(bits), _is_signed(is_signed) {
}

code::FloatType::FloatType(unsigned bits)
  : bits(bits) {
}

code::FunctionType::FunctionType(std::shared_ptr<code::Type>  result)
  : result(result), parameters{} {
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

LLVMTypeRef code::FunctionType::handle() const noexcept {
  // Determine the result type (either void or declared)
  // TODO: Full body deduction should come eventually
  auto res = result ? result->handle() : LLVMVoidType();

  // Collect the type handles for all paramters
  std::vector<LLVMTypeRef> params;
  params.reserve(parameters.size());
  for (auto& p : parameters) {
    params.push_back(p->handle());
  }

  return LLVMFunctionType(res, params.data(), params.size(), false);
}

LLVMTypeRef code::StringType::handle() const noexcept {
  return LLVMPointerType(LLVMIntType(8), 0);
}
