// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_CODE_SLOT_H
#define ARROW_CODE_SLOT_H 1

#include "arrow/llvm.hpp"
#include "arrow/code/item.hpp"

namespace arrow {
namespace code {

struct Slot : Item {
  Slot(ast::Node* context, std::string name, bool is_mutable)
    : Item(context, name),
      is_mutable(is_mutable),
      type(nullptr),
      handle(nullptr) {
  }

  virtual ~Slot() noexcept;

  bool is_mutable;
  Ref<code::Type> type;
  LLVMValueRef handle;
};

}  // namespace code
}  // namespace arrow

#endif  // ARROW_CODE_SLOT_H
