// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/expose.hpp"
#include "arrow/generator.hpp"
#include "arrow/log.hpp"

using arrow::Expose;
namespace code = arrow::code;
namespace ast = arrow::ast;

Expose::Expose(arrow::Generator& g, code::Scope& scope)
  : _g{g}, _scope{scope} {
}

Expose::~Expose() noexcept { }

void Expose::visit_function(ast::Function& x) {
  auto& name = x.name->text;

  // FIXME: auto type = resolve(node)
  auto type = LLVMFunctionType(LLVMVoidType(), nullptr, 0, false);
  auto handle = LLVMAddFunction(_g._mod, name.c_str(), type);

  // Create and set the new function item in the scope
  _scope.set(name, std::make_shared<code::Function>(
    handle,
    name,
    &_scope));
}
