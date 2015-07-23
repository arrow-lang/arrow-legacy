// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/build.hpp"

namespace arrow {
namespace pass {

void Build::visit_function(ast::Function& x) {
  // Pull out the previously-exposed item
  // TODO(mehcode): `scope->find<T>`
  auto item = _scope->find(&x).as<code::Function>();
  if (!item) return;

  // Create the actual LLVM function
  auto type = item->type.as<code::TypeFunction>();
  auto type_handle = LLVMGetElementType(type->handle());
  auto handle = LLVMAddFunction(_ctx.mod, item->name.c_str(), type_handle);
  item->set_address(handle);

  // TODO(mehcode): If we're building a library and this is exported..
  LLVMSetLinkage(handle, LLVMInternalLinkage);

  // Add the top basic block
  auto last_block = LLVMGetInsertBlock(_ctx.irb);
  auto block = LLVMAppendBasicBlock(handle, "");
  LLVMPositionBuilderAtEnd(_ctx.irb, block);

  // Has the function been terminated?
  if (!LLVMGetBasicBlockTerminator(LLVMGetLastBasicBlock(handle))) {
    // No; we need to terminate
    if (type->result.is<code::TypeNone>()) {
      // No result type
      LLVMBuildRetVoid(_ctx.irb);
    // TODO(mehcode): Should really check if errors occured for this function
    } else if (Log::get().count("error") == 0) {
      // We should have gotten a result; report an error and exit
      Log::get().error(x.span, "not all code paths return a value");
      return;
    }
  }

  // Move instruction ptr back to where it was (if it was somewhere)
  if (last_block) {
    LLVMPositionBuilderAtEnd(_ctx.irb, last_block);
  }

  // // Expose the module block (into the new module scope).
  // Expose(_ctx, item->scope).run(*x.block);
  // if (Log::get().count("error") > 0) return;
  //
  // // Analyze (usage analysis) the module block.
  // AnalyzeUsage(item->scope).run(*x.block);
  // if (Log::get().count("error") > 0) return;
  //
  // // Analyze (type) the module block.
  // AnalyzeType(item->scope).run(*x.block);
  // if (Log::get().count("error") > 0) return;
  //
  // // Add the module initializer basic block
  // auto last_block = LLVMGetInsertBlock(_ctx.irb);
  // auto block = LLVMAppendBasicBlock(mod_init_fn, "");
  // LLVMPositionBuilderAtEnd(_ctx.irb, block);

  // Visit the module block with the builder.
  // Build(_ctx, item->scope).run(*x.block);
  // if (Log::get().count("error") > 0) return;

  // // Terminate the module initializer
  // LLVMBuildRetVoid(_ctx.irb);
  //
  // // Move instruction ptr back to where it was (if it was somewhere)
  // if (last_block) {
  //   LLVMPositionBuilderAtEnd(_ctx.irb, last_block);
  // }
  //
  // // Leave the module scope-block
  // _scope->exit();
}

}  // namespace pass
}  // namespace arrow
