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

// TODO: Functions

// TODO(mehcode): Make this a value as well
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
