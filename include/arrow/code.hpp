// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_CODE_H
#define ARROW_CODE_H 1

#include <string>

#include "arrow/llvm.hpp"
#include "arrow/code/item.hpp"
#include "arrow/code/scope.hpp"
#include "arrow/code/type.hpp"
#include "arrow/code/value.hpp"

namespace arrow {
namespace code {

/// A module, either top-level (a file) or a sub-module block inside
/// the file
struct Module : Item {
  Module(const std::string& name, Scope* parent);

  virtual ~Module() noexcept;

  Scope scope;
  std::string name;
};

/// A named function definition
struct Function : Item {
  Function(LLVMValueRef handle, const std::string& name, Scope* parent);

  virtual ~Function() noexcept;

  virtual LLVMValueRef handle() noexcept {
    return _handle;
  }

  std::string name;
  Scope scope;

 private:
  LLVMValueRef _handle;
};

/// A named slot declaration
struct Slot : Value {
  Slot(
    const std::string& name, LLVMValueRef handle, std::shared_ptr<Type> type);

  virtual ~Slot() noexcept;

  virtual bool has_address() const noexcept {
    return true;
  }

  std::string name;
};

}  // namespace code
}  // namespace arrow

#endif  // ARROW_CODE_H
