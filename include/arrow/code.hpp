// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_CODE_H
#define ARROW_CODE_H 1

#include "arrow/code/item.hpp"
#include "arrow/code/scope.hpp"
#include "arrow/llvm.hpp"

namespace arrow {
namespace code {

/// A module, either top-level (a file) or a sub-module block inside
/// the file
struct Module : Item {
  explicit Module(const std::string& name);

  virtual ~Module() noexcept;

  Scope scope;
  std::string name;
};

/// A named function definition
struct Function : Item {
  explicit Function(LLVMValueRef handle, const std::string& name);

  virtual ~Function() noexcept;

  virtual LLVMValueRef handle() noexcept {
    return _handle;
  }

  std::string name;

private:
  LLVMValueRef _handle;
};

} // namespace code {
} // namespace arrow {

#endif // ARROW_CODE_H
