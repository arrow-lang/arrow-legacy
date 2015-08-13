// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/declare.hpp"

namespace arrow {
namespace pass {

void Declare::visit_struct(ast::Structure& x) {
  // Pull out the previously-exposed item
  // TODO(mehcode): `scope->find<T>`
  auto item = _scope->find(&x).as<code::Structure>();
  if (!item) return;

  // Create the opaque type handle
  auto type_handle = LLVMStructCreateNamed(
    LLVMGetGlobalContext(), item->name.c_str());

  // Build and store the type
  item->type->set_handle(type_handle);
}

}  // namespace pass
}  // namespace arrow
