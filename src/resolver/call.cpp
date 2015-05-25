// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/resolver.hpp"
#include "arrow/log.hpp"

using arrow::Resolver;
using arrow::resolve;

void Resolver::visit(ast::Call& x) {
  // Attempt to resolve the function that will be called
  // TODO(mehcode): Function "values"
  // TODO(mehcode): resolve<T>
  auto item = resolve(_g, _scope, *x.expression);
  if (!item) { return; }
  if (!item->is<code::FunctionType>()) {
    // Not a function; lame
    // TODO(mehcode): Function "values" and some kind of error message here
    return;
  }

  auto& fn = item->as<code::FunctionType>();
  if (!fn.result) {
    // This doesn't have a return type; not necessarily an error, yet.
    return;
  }

  _stack.push(fn.result);
}
