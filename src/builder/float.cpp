// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/builder.hpp"
#include "arrow/resolver.hpp"

using arrow::Builder;
using arrow::resolve;

void Builder::visit_float(ast::Float& x) {
  // Resolve the specific type of float
  auto type = resolve(_g, *_cs, x);

  // Build the value handle
  auto handle = LLVMConstRealOfStringAndSize(
    type->handle(_g), x.text.c_str(), x.text.size());

  // Build and push the code handle
  _stack.push(std::make_shared<code::Value>(&x, _cs, handle, type));
}
