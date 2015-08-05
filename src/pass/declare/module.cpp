// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/declare.hpp"

namespace arrow {
namespace pass {

void Declare::visit_module(ast::Module& x) {
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

  // Enter the module-scope block
  item->scope->enter(&x);

  // Declare any items that need forward declarations.
  Declare(_ctx, item->scope).run(*x.block);

  // Leave the module scope-block
  item->scope->exit();
}

}  // namespace pass
}  // namespace arrow
