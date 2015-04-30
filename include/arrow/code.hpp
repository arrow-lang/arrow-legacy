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
  Function(LLVMValueRef handle, const std::string& name);

  virtual ~Function() noexcept;

  virtual LLVMValueRef handle() noexcept {
    return _handle;
  }

  std::string name;

 private:
  LLVMValueRef _handle;
};

/// A named slot declaration
struct Slot : Item {
  Slot(LLVMValueRef handle, const std::string& name);

  virtual ~Slot() noexcept;

  virtual LLVMValueRef handle() noexcept {
    return _handle;
  }

  std::string name;

 private:
  LLVMValueRef _handle;
};

}  // namespace code
}  // namespace arrow

#endif  // ARROW_CODE_H
