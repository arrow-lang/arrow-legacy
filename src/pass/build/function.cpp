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

  auto type = item->type.as<code::TypeFunction>();
  auto handle = item->get_value(_ctx);

  // Add the top basic block
  auto last_block = LLVMGetInsertBlock(_ctx.irb);
  auto block = LLVMAppendBasicBlock(handle, "");
  LLVMPositionBuilderAtEnd(_ctx.irb, block);

  // Build the function body
  Build(_ctx, item->scope).run(*x.block);
  if (Log::get().count("error") > 0) return;

  // Has the function been terminated?
  if (!LLVMGetBasicBlockTerminator(LLVMGetLastBasicBlock(handle))) {
    // No; we need to terminate
    if (!type->result || type->result.is<code::TypeNone>()) {
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
}

}  // namespace pass
}  // namespace arrow
