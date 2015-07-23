// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_CODE_FUNCTION_H
#define ARROW_CODE_FUNCTION_H 1

#include "arrow/llvm.hpp"
#include "arrow/code/item.hpp"
#include "arrow/code/value.hpp"

namespace arrow {
namespace code {

struct Function : Item, Value {
  Function(ast::Node* context, std::string name, Ref<code::Scope> parent_scope)
    : Item(context, name),
      Value(nullptr, nullptr),
      scope(parent_scope) {
  }

  virtual ~Function() noexcept;

  void set_address(LLVMValueRef handle) {
    _handle = handle;
  }

  virtual bool has_address() const {
    return false;
  }

  /// Function scope
  Ref<code::Scope> scope;
};

struct ExternFunction : Item, Value {
  ExternFunction(ast::Node* context, std::string name, Ref<TypeFunction> type)
    : Item(context, name),
      Value(nullptr, type) {
  }

  virtual ~ExternFunction() noexcept;

  virtual LLVMValueRef get_value(Compiler::Context& ctx);

  virtual bool has_address() const {
    return false;
  }
};

}  // namespace code
}  // namespace arrow

#endif  // ARROW_CODE_FUNCTION_H
