// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/code/type.hpp"

namespace arrow {
namespace code {

LLVMTypeRef TypeBoolean::handle() {
  return LLVMInt1Type();
}

LLVMTypeRef TypeFloat::handle() {
  return LLVMDoubleType();
}

LLVMTypeRef TypeInteger::handle() {
  return LLVMIntType(bits);
}

LLVMTypeRef TypeTuple::handle() {
  if (!_handle) {
    std::vector<LLVMTypeRef> element_types;
    element_types.reserve(elements.size());
    for (auto& element : elements) {
      element_types.push_back(element->handle());
    }

    _handle = LLVMStructType(
      element_types.data(), element_types.size(), false);
  }

  return _handle;
}

}  // namespace code
}  // namespace arrow
