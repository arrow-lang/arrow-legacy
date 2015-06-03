// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/resolver.hpp"
#include "arrow/log.hpp"

using arrow::Resolver;
using arrow::resolve;

void Resolver::do_function(ast::AbstractFunction& x) {
  // Resolve the result type (if defined)
  std::shared_ptr<code::Type> result = nullptr;
  if (x.result) {
    result = resolve(_g, _scope, *x.result);
    if (!result) { return; }
  }

  auto type = std::make_shared<code::FunctionType>(result);

  // Resolve the type of each parameter (if any)
  // TODO(mehcode): Use the visitor
  for (auto& param : x.parameters) {
    auto param_type = resolve(_g, _scope, *(param->type));
    if (!param_type) { return; }

    type->parameters.push_back(param_type);
  }

  _stack.push(type);
}

void Resolver::visit_function(ast::Function& x) {
  do_function(x);
}

void Resolver::visit_extern_function(ast::ExternalFunction& x) {
  // TODO(mehcode): Handle ABI promotion
  //  - int8 > int32 (because C said)
  do_function(x);
}
