// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/expose.hpp"
#include "arrow/resolver.hpp"
#include "arrow/generator.hpp"
#include "arrow/log.hpp"

using arrow::resolve;
using arrow::Expose;
namespace code = arrow::code;
namespace ast = arrow::ast;

Expose::Expose(arrow::Generator& g, code::Scope& scope)
  : _g{g}, _scope{scope} {
}

Expose::~Expose() noexcept { }

void Expose::visit_function(ast::Function& x) {
  // Resolve the type of this function
  auto type = resolve(_g, _scope, x);
  if (!type) { return; }

  auto& name = x.name->text;
  auto handle = LLVMAddFunction(_g._mod, name.c_str(), type->handle());

  // Create and set the new function item in the scope
  // TODO: Functions should receive module scope
  _scope.set(name, std::make_shared<code::Function>(
    handle,
    type,
    name,
    &_scope));
}
