// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/exposer.hpp"
#include "arrow/resolver.hpp"
#include "arrow/generator.hpp"
#include "arrow/log.hpp"

using arrow::resolve;
using arrow::Exposer;
namespace code = arrow::code;
namespace ast = arrow::ast;

void Exposer::visit_slot(ast::Slot& x) {
  auto& name = x.name->text;

  // Check if we are overwriting an item in the current scope
  // As this is STATIC allocation (for the moment); this is a hard error
  if (_scope.exists(name, false)) {
    Log::get().error(
      x.name->span, "redefinition of static '%s'", name.c_str());
  }

  // Resolve the type of the slot
  std::shared_ptr<code::Type> type;
  if (x.type) {
    type = resolve(_g, _scope, *x.type);
  } else {
    type = resolve(_g, _scope, *x.initializer);
  }

  // Add the global to LLVM
  auto handle = LLVMAddGlobal(_g._mod, type->handle(_g), name.c_str());

  // Create (and set) the slot item
  _scope.set(name, std::make_shared<code::Slot>(
    &x,
    &_scope,
    name,
    handle,
    type,
    x.is_mutable));
}
