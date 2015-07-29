// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/declare.hpp"

namespace arrow {
namespace pass {

void Declare::visit_function(ast::Function& x) {
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
}

}  // namespace pass
}  // namespace arrow
