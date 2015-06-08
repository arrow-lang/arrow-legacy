// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/resolver.hpp"
#include "arrow/log.hpp"

using arrow::Resolver;
using arrow::resolve;

void Resolver::visit_struct(ast::Structure& x) {
  // Create the initial type item
  // TODO: This should be FULLY QUALIFIED name
  auto type = std::make_shared<code::StructureType>(&x, x.name->text);

  // Enumerate through each member and resolve their types
  for (auto& mem : x.members) {
    auto mem_type = resolve(_g, _scope, *(mem->type));
    if (!mem_type) { return; }

    // Push the member onto the type
    type->members.push_back(std::make_shared<code::StructureMember>(
      mem.get(),
      mem->name->text,
      mem_type
    ));
  }

  _stack.push(type);
}
