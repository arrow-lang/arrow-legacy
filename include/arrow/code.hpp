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

class Generator;

namespace code {

/// A module, either top-level (a file) or a sub-module block inside
/// the file
struct Module : Item {
  Module(ast::Node* context,
         const std::string& name, Scope* parent);

  virtual ~Module() noexcept;

  Scope scope;
  std::string name;
  LLVMValueRef function;
};

struct AbstractFunction : Item {
  AbstractFunction(
    ast::Node* context,
    code::Scope* scope,
    std::shared_ptr<FunctionType> type, const std::string& name);

  virtual ~AbstractFunction() noexcept;

  virtual LLVMValueRef handle(Generator& g) noexcept = 0;

  virtual std::shared_ptr<FunctionType> type() const noexcept {
    return _type;
  }

  std::string name;

 protected:
  std::shared_ptr<FunctionType> _type;
};

/// A named function definition
struct Function : AbstractFunction {
  Function(
    ast::Node* context,
    LLVMValueRef handle, std::shared_ptr<FunctionType> type,
    const std::string& name, Scope* parent);

  virtual ~Function() noexcept;

  virtual LLVMValueRef handle(Generator&) noexcept {
    return _handle;
  }

  Scope scope;

 private:
  LLVMValueRef _handle;
};

/// An external function declaration
struct ExternalFunction : AbstractFunction {
  ExternalFunction(
    ast::Node* context,
    code::Scope* scope,
    LLVMModuleRef _mod, std::shared_ptr<FunctionType> type,
    const std::string& name);

  virtual ~ExternalFunction() noexcept;

  virtual LLVMValueRef handle(Generator& g) noexcept;

 private:
  LLVMModuleRef _mod;
  LLVMValueRef _handle;
};

/// A named slot declaration
struct Slot : Value {
  Slot(
    ast::Node* context,
    code::Scope* scope,
    const std::string& name, LLVMValueRef handle,
    std::shared_ptr<Type> type,
    bool _mutable);

  virtual ~Slot() noexcept;

  std::string name;
};

/// A named structure declaration
struct Structure : Item {
  Structure(
    ast::Node* context,
    code::Scope* scope,
    const std::string& name,
    std::shared_ptr<StructureType> type);

  virtual ~Structure() noexcept;

  virtual std::shared_ptr<StructureType> type() const noexcept {
    return _type;
  }

  std::string name;

 private:
  std::shared_ptr<StructureType> _type;
};

}  // namespace code
}  // namespace arrow

#endif  // ARROW_CODE_H
