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

void Exposer::visit_function(ast::Function& x) {
  // Resolve the type of this function
  auto type = std::static_pointer_cast<code::FunctionType>(
    resolve(_g, _scope, x));
  if (!type) { return; }

  auto& name = x.name->text;
  auto namespace_ = _scope.name();

  auto handle = LLVMAddFunction(
    _g._mod, (namespace_ + "." + name).c_str(), type->handle(_g));

  // Create and set the new function item in the scope
  // TODO(mehcode): Functions should receive module scope
  _scope.set(name, std::make_shared<code::Function>(
    &x,
    handle,
    type,
    name,
    &_scope));
}

void Exposer::visit_extern_function(ast::ExternalFunction& x) {
  // Resolve the type of this function
  auto type = std::static_pointer_cast<code::FunctionType>(
    resolve(_g, _scope, x));
  if (!type) { return; }

  // Create and set the new function item in the scope
  _scope.set(x.name->text, std::make_shared<code::ExternalFunction>(
    &x,
    &_scope,
    _g._mod,
    type,
    x.name->text));
}
