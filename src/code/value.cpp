// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/code/value.hpp"
#include "arrow/generator.hpp"

using arrow::code::Value;

Value::Value(LLVMValueRef handle, std::shared_ptr<Type> type)
  : _handle{handle}, _type{type} {
  std::printf("build Value with %p\n", type.get());
}

Value::~Value() noexcept {
}

auto Value::type() const noexcept -> std::shared_ptr<Type> {
  return _type;
}

LLVMValueRef Value::value_of(Generator& g) const noexcept {
  if (has_address()) {
    return LLVMBuildLoad(g._irb, _handle, "");
  } else {
    return _handle;
  }
}

LLVMValueRef Value::address_of(Generator& g) const noexcept {
  if (has_address()) {
    return _handle;
  } else {
    // TODO: Do the conversion
    return nullptr;
  }
}
