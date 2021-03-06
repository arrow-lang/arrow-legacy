// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/build.hpp"
#include "arrow/pass/expose.hpp"
#include "arrow/pass/declare.hpp"
#include "arrow/pass/analyze-usage.hpp"
#include "arrow/pass/analyze-type.hpp"

namespace arrow {
namespace pass {

void Build::visit_module(ast::Module& x) {
  // Get the existing module-item
  auto item = _ctx.modules_by_context[&x];
  if (!item) return;

  // Activate the module initializer
  auto last_block = LLVMGetInsertBlock(_ctx.irb);
  auto block = LLVMGetLastBasicBlock(item->initializer);
  LLVMPositionBuilderAtEnd(_ctx.irb, block);

  // Visit the module block with the builder.
  Build(_ctx, item->scope).run(*x.block);
  if (Log::get().count("error") > 0) return;

  // Terminate the module initializer
  LLVMBuildRetVoid(_ctx.irb);

  // Move instruction ptr back to where it was (if it was somewhere)
  if (last_block) {
    LLVMPositionBuilderAtEnd(_ctx.irb, last_block);
  }
}

}  // namespace pass
}  // namespace arrow
