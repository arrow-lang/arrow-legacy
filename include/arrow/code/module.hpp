// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_CODE_MODULE_H
#define ARROW_CODE_MODULE_H 1

#include "arrow/code/item.hpp"

namespace arrow {
namespace code {

struct Module : Item {
  Module(
    ast::Node* context,
    std::string name,
    LLVMValueRef initializer,
    Ref<code::Scope> parent_scope
  )
    : Item(context, name), initializer(initializer), scope(parent_scope) {
  }

  virtual ~Module() noexcept;

  /// Module initializer.
  // TODO(mehcode): Use code::Function when available
  LLVMValueRef initializer;

  /// Module scope
  Ref<code::Scope> scope;
};

}  // namespace code
}  // namespace arrow

#endif  // ARROW_CODE_MODULE_H
