// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/code/value.hpp"

using arrow::code::Value;

Value::Value(LLVMValueRef handle, std::shared_ptr<Type> type)
  : _handle{handle}, _type{type} {
}

Value::~Value() noexcept {
}

LLVMValueRef Value::handle() const noexcept {
  return _handle;
}

auto Value::type() const noexcept -> std::shared_ptr<Type> {
  return _type;
}
