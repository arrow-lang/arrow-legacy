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

  // Create the LLVM module initialization function
  auto mod_init_name = x.name + "..init";
  auto mod_init_ty = LLVMFunctionType(LLVMVoidType(), nullptr, 0, false);
  auto mod_init_fn = LLVMAddFunction(
    _ctx.mod, mod_init_name.c_str(), mod_init_ty);

  // TODO(mehcode): If we're building a library and this is exported..
  LLVMSetLinkage(mod_init_fn, LLVMInternalLinkage);

  // Set the module initializer
  item->initializer = mod_init_fn;

  // // Expose the module block (into the new module scope).
  // Expose(_ctx, item->scope).run(*x.block);
  // if (Log::get().count("error") > 0) return;
  //
  // // Analyze (type) the module block.
  // AnalyzeType(item->scope).run(*x.block);
  // if (Log::get().count("error") > 0) return;
  //
  // // Analyze (usage analysis) the module block.
  // AnalyzeUsage(item->scope).run(*x.block);
  // if (Log::get().count("error") > 0) return;
  //
  // // Declare any items that need forward declarations.
  // Declare(_ctx, item->scope).run(*x.block);
  // if (Log::get().count("error") > 0) return;

  // Add the module initializer basic block
  auto last_block = LLVMGetInsertBlock(_ctx.irb);
  auto block = LLVMAppendBasicBlock(mod_init_fn, "");
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
