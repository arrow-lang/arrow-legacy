// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/builder.hpp"
#include "arrow/resolver.hpp"
#include "arrow/generator.hpp"

using arrow::Builder;
using arrow::resolve;

void Builder::visit_slot(ast::Slot& x) {
  auto& name = x.name->text;

  // Check if we are currently outside of a function (which would mean
  // this is a global)
  if (_cf == nullptr) {
    // Get the global item
    auto item = _cs->get(name);

    if (x.initializer) {
      // Activate the initialization function
      LLVMPositionBuilderAtEnd(_g._irb, LLVMGetLastBasicBlock(_cm->function));

      // Build the initializer
      auto init = build_scalar_of<code::Value>(*x.initializer);

      // Create a store for the initializer
      // If this is a constant; just set the global var initializer
      auto& ptr = item->as<code::Slot>();
      auto val = init->value_of(_g);
      if (LLVMIsConstant(val)) {
        LLVMSetInitializer(ptr.address_of(_g), val);

        if (!x.is_mutable) {
          LLVMSetGlobalConstant(ptr.address_of(_g), true);
        }
      } else {
        LLVMSetInitializer(
          ptr.address_of(_g), LLVMConstNull(ptr.type()->handle(_g)));

        LLVMBuildStore(_g._irb, val, ptr.address_of(_g));
      }
    }

    return;
  }

  // Check if we are overwriting an item in the current scope
  // NOTE: We need to eventually decide if we will allow this or not
  //  I'm leaning towards allowing as long as we output a warning for
  //  an unused variable
  if (_cs->exists(name, false)) {
    Log::get().warning(x.name->span, "redefinition of '%s'", name.c_str());
  }

  // Build the initializer expression (only if we have one)
  std::shared_ptr<code::Value> initializer = nullptr;
  std::shared_ptr<code::Type> type;
  if (x.initializer) {
    type = resolve(_g, *_cs, *x.initializer);
    if (!type) return;

    initializer = build_scalar_of<code::Value>(*x.initializer);
    if (!initializer) return;
  }

  // Use the declared type (if present)
  if (x.type != nullptr) {
    type = build_type(*x.type);
    if (!type) return;

    // Perform the cast (if we have an initializer)
    if (initializer) {
      initializer = initializer->cast(_g, *x.initializer, type);
      if (!initializer) return;
    }
  }

  // Build the allocation
  auto handle = LLVMBuildAlloca(_g._irb, type->handle(_g), name.c_str());

  // Create and set the new slot decl in
  // the current scope
  _cs->set(name, std::make_shared<code::Slot>(
    &x,
    _cs,
    name,
    handle,
    type,
    x.is_mutable));

  if (initializer) {
    // TODO(mehcode): Extract into an `assign` function
    // Create a store for the initializer
    LLVMBuildStore(_g._irb, initializer->value_of(_g), handle);
  }
}
