// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/code.hpp"

namespace code = arrow::code;

#define IMPL(N) \
  code::N::~N() noexcept { }

IMPL(Module)
IMPL(Function)
IMPL(Slot)

code::Module::Module(const std::string& name, Scope* parent)
  : scope{parent}, name{name} {
}

code::Function::Function(LLVMValueRef handle, const std::string& name, Scope* parent)
  : name{name}, scope{parent}, _handle{handle} {
}

code::Slot::Slot(const std::string& name, LLVMValueRef handle, std::shared_ptr<Type> type)
  : code::Value(handle, type), name{name} {
}
