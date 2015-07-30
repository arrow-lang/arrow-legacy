// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_CODE_MODULE_H
#define ARROW_CODE_MODULE_H 1

#include "arrow/code/item.hpp"
#include "arrow/ast/nodes.hpp"

namespace arrow {
namespace code {

struct Module {
  Module(
    Ref<ast::Module> context,
    std::string name,
    LLVMValueRef initializer,
    Ref<code::Scope> parent_scope
  )
    : context(context), name(name), initializer(initializer),
      scope(new Scope(name, parent_scope, dynamic_cast<Item*>(this))) {
  }

  virtual ~Module() noexcept;

  /// AST node from whence this item came from (the source file).
  Ref<ast::Module> context;

  /// Name of the declaration item.
  std::string name;

  /// Module initializer.
  // TODO(mehcode): Use code::Function when available
  LLVMValueRef initializer;

  /// Module scope
  Ref<code::Scope> scope;
};

struct Import : Item {
  Import(ast::Node* context, std::string name, Ref<code::Module> module)
    : Item(context, name), module(module) {
  }

  virtual ~Import() noexcept;

  /// Module
  Ref<code::Module> module;
};

}  // namespace code
}  // namespace arrow

#endif  // ARROW_CODE_MODULE_H
