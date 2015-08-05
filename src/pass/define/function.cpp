// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/define.hpp"

namespace arrow {
namespace pass {

void Define::visit_function(ast::Function& x) {
  // Pull out the previously-exposed item
  // TODO(mehcode): `scope->find<T>`
  auto item = _scope->find(&x).as<code::Function>();
  if (!item) return;

  // Add the top basic block
  auto handle = item->get_value(_ctx);
  LLVMAppendBasicBlock(handle, "");
}

}  // namespace pass
}  // namespace arrow
