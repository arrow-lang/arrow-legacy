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
IMPL(Structure)

code::Module::Module(ast::Node* context, const std::string& name, Scope* parent)
  : Item(context, parent), scope{name, parent}, name{name} {
}

code::AbstractFunction::AbstractFunction(
  ast::Node* context,
  Scope* scope,
  std::shared_ptr<FunctionType> type,
  const std::string& name
)
  : Item(context, scope), name{name}, _type{type} {
}

code::Function::Function(
  ast::Node* context,
  LLVMValueRef handle,
  std::shared_ptr<FunctionType> type,
  const std::string& name,
  Scope* parent
)
  : AbstractFunction(context, parent, type, name), scope{name, parent}, _handle{handle} {
}

code::Structure::Structure(
  ast::Node* context,
  Scope* scope,
  const std::string& name,
  std::shared_ptr<StructureType> type
)
  : Item(context, scope), name(name), _type(type) {
}

code::ExternalFunction::ExternalFunction(
  ast::Node* context,
  Scope* scope,
  LLVMModuleRef _mod,
  std::shared_ptr<FunctionType> type,
  const std::string& name
)
  : AbstractFunction(context, scope, type, name), _mod{_mod}, _handle{nullptr} {
}

code::Slot::Slot(
  ast::Node* context,
  Scope* scope,
  const std::string& name, LLVMValueRef handle,
  std::shared_ptr<Type> type,
  bool _mutable
)
  : code::Value(context, scope, handle, type, _mutable, true), name{name} {
}

LLVMValueRef code::ExternalFunction::handle(Generator& g) noexcept {
  if (_handle == nullptr) {
    _handle = LLVMAddFunction(_mod, name.c_str(), _type->handle(g));
  }

  return _handle;
}
