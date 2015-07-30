// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_CODE_FUNCTION_H
#define ARROW_CODE_FUNCTION_H 1

#include "arrow/llvm.hpp"
#include "arrow/code/item.hpp"
#include "arrow/code/container.hpp"
#include "arrow/code/value.hpp"

namespace arrow {
namespace code {

struct Parameter : Item, Value {
  Parameter(ast::Node* context, std::string name, bool is_mutable)
    : Item(context, name),
      Value(nullptr, nullptr),
      is_mutable(is_mutable) {
  }

  virtual ~Parameter() noexcept;

  void set_address(LLVMValueRef handle) {
    _handle = handle;
  }

  virtual bool has_address() const {
    return true;
  }

  /// Whether this is mutable or immutable.
  bool is_mutable;
};

struct Function : Item, Value, Container {
  Function(ast::Node* context, std::string name, Ref<code::Scope> parent_scope)
    : Item(context, name),
      Value(nullptr, nullptr),
      Container(name, parent_scope) {
  }

  virtual ~Function() noexcept;

  void set_address(LLVMValueRef handle) {
    _handle = handle;
  }

  virtual bool has_address() const {
    return false;
  }
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
