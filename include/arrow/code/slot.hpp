// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_CODE_SLOT_H
#define ARROW_CODE_SLOT_H 1

#include <unordered_set>

#include "arrow/llvm.hpp"
#include "arrow/code/item.hpp"
#include "arrow/code/value.hpp"

namespace arrow {
namespace code {

struct Slot : Item, Value {
  Slot(ast::Node* context, std::string name, bool is_mutable)
    : Item(context, name),
      Value(nullptr, nullptr),
      is_mutable(is_mutable) {
  }

  virtual ~Slot() noexcept;

  void set_address(LLVMValueRef handle) {
    _handle = handle;
  }

  virtual bool has_address() const {
    return true;
  }

  /// Whether this is mutable or immutable.
  bool is_mutable;
};

struct ExternSlot : Item, Value {
  ExternSlot(
    ast::Node* context, std::string name, Ref<code::Type> type, bool is_mutable
  )
    : Item(context, name),
      Value(nullptr, type),
      is_mutable(is_mutable) {
  }

  virtual ~ExternSlot() noexcept;

  virtual LLVMValueRef get_value(Compiler::Context& ctx);
  virtual LLVMValueRef get_address(Compiler::Context& ctx);

  void set_address(LLVMValueRef handle) {
    _handle = handle;
  }

  virtual bool has_address() const {
    return true;
  }

  bool is_mutable;

 private:
  LLVMValueRef handle(Compiler::Context& ctx);
};

}  // namespace code
}  // namespace arrow

#endif  // ARROW_CODE_SLOT_H
