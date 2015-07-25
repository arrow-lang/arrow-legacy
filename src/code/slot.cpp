// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/code/slot.hpp"

namespace arrow {
namespace code {

// Slot
// -----------------------------------------------------------------------------

void Slot::add_assignment(Ref<code::Block> block, bool is_definite) {
  _assign[block] = is_definite;
}

bool* Slot::is_assigned(Ref<code::Block> block) {
  // Check for an assignment in /this/ block
  if (_assign.find(block) != _assign.end()) {
    return &(_assign[block]);
  }

  // Iterate up the block's ancestry to determine if
  // it was assigned in a prior block
  auto parent = block->parent();
  if (parent != nullptr) {
    return is_assigned(parent);
  }

  // Cannot find an assignment
  return nullptr;
}

bool Slot::is_local(Ref<code::Scope> scope) {
  return !!scope->find(context, true, true, false);
}

// External Slot
// -----------------------------------------------------------------------------

LLVMValueRef ExternSlot::handle(Compiler::Context& ctx) {
  if (!_handle) {
    _handle = LLVMAddGlobal(ctx.mod, type->handle(), name.c_str());
    LLVMSetExternallyInitialized(_handle, true);
  }

  return _handle;
}

LLVMValueRef ExternSlot::get_value(Compiler::Context& ctx) {
  return LLVMBuildLoad(ctx.irb, handle(ctx), "");
}

LLVMValueRef ExternSlot::get_address(Compiler::Context& ctx) {
  return handle(ctx);
}

}  // namespace code
}  // namespace arrow
