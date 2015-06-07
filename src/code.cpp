// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <string>
#include "arrow/code.hpp"

namespace code = arrow::code;

#define IMPL(N) \
  code::N::~N() noexcept { }

IMPL(Module)
IMPL(AbstractFunction)
IMPL(Function)
IMPL(ExternalFunction)
IMPL(Slot)

code::Module::Module(std::shared_ptr<ast::Node> context, const std::string& name, Scope* parent)
  : Item(context), scope{name, parent}, name{name} {
}

code::AbstractFunction::AbstractFunction(
  std::shared_ptr<ast::Node> context,
  std::shared_ptr<FunctionType> type,
  const std::string& name
)
  : Item(context), name{name}, _type{type} {
}

code::Function::Function(
  std::shared_ptr<ast::Node> context,
  LLVMValueRef handle,
  std::shared_ptr<FunctionType> type,
  const std::string& name,
  Scope* parent
)
  : AbstractFunction(context, type, name), scope{name, parent}, _handle{handle} {
}

code::ExternalFunction::ExternalFunction(
  std::shared_ptr<ast::Node> context,
  LLVMModuleRef _mod,
  std::shared_ptr<FunctionType> type,
  const std::string& name
)
  : AbstractFunction(context, type, name), _mod{_mod}, _handle{nullptr} {
}

code::Slot::Slot(
  std::shared_ptr<ast::Node> context,
  const std::string& name, LLVMValueRef handle,
  std::shared_ptr<Type> type,
  bool _mutable
)
  : code::Value(context, handle, type, _mutable, true), name{name} {
}

LLVMValueRef code::ExternalFunction::handle() noexcept {
  if (_handle == nullptr) {
    _handle = LLVMAddFunction(_mod, name.c_str(), _type->handle());
  }

  return _handle;
}
