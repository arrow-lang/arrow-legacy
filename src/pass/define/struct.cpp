// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/define.hpp"
#include "arrow/match.hpp"

namespace arrow {
namespace pass {

void Define::visit_struct(ast::Structure& x) {
  // Pull out the previously-exposed item
  auto item = _scope->find(&x).as<code::Structure>();
  if (!item) return;

  // Iterate through each member and build the member type
  std::vector<LLVMTypeRef> elements;
  elements.reserve(item->type->members.size());
  for (auto& mem : item->type->members) {
    elements.push_back(mem->type->handle());
  }

  // Set the body of the structure
  LLVMStructSetBody(
    item->type->handle(), elements.data(), elements.size(), false);
}

}  // namespace pass
}  // namespace arrow
