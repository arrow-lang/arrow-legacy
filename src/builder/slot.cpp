// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/builder.hpp"
#include "arrow/resolver.hpp"
#include "arrow/generator.hpp"

using arrow::Builder;
using arrow::resolve;

void Builder::visit(ast::Slot& x) {
  auto& name = x.name->text;

  // Check if we are overwriting an item in the current scope
  // NOTE: We need to eventually decide if we will allow this or not
  //  I'm leaning towards allowing as long as we output a warning for
  //  an unused variable
  if (_cs->exists(name, false)) {
    Log::get().warning(x.name->span, "redefinition of '%s'", name.c_str());
  }

  // Build the initializer expression

  // TODO: Only if we have one
  auto initializer = build_scalar_of<code::Value>(*x.initializer);
  if (!initializer) return;
  auto type = initializer->type();

  if (x.type != nullptr) {
    type = build_scalar_of<code::Type>(*x.type);
    if (!type) return;

    // TODO: Check for type mis-match
  }

  auto handle = LLVMBuildAlloca(_g._irb, type->handle(), name.c_str());

  // Create and set the new slot decl in
  // the current scope
  _cs->set(name, std::make_shared<code::Slot>(
    name,
    handle,
    type));

  if (initializer) {
    // TODO: Extract into an `assign` function
    // Create a store for the initializer
    LLVMBuildStore(_g._irb, initializer->value_of(_g), handle);
  }
}