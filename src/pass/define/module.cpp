// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/define.hpp"

namespace arrow {
namespace pass {

void Define::visit_module(ast::Module& x) {
  // Get the existing module-item
  auto item = _ctx.modules_by_context[&x];
  if (!item) return;

  // Add the module initializer basic block
  auto last_block = LLVMGetInsertBlock(_ctx.irb);
  auto block = LLVMAppendBasicBlock(item->initializer, "");
  LLVMPositionBuilderAtEnd(_ctx.irb, block);

  // Enter the module-scope block
  item->scope->enter(&x);

  // Define any items that need forward declarations.
  Define(_ctx, item->scope).run(*x.block);

  // Leave the module scope-block
  item->scope->exit();

  // Move instruction ptr back to where it was (if it was somewhere)
  if (last_block) {
    LLVMPositionBuilderAtEnd(_ctx.irb, last_block);
  }
}

}  // namespace pass
}  // namespace arrow
