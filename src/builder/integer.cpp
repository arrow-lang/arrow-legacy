// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/builder.hpp"
#include "arrow/resolver.hpp"

using arrow::Builder;
using arrow::resolve;

void Builder::visit(ast::Integer& x) {
  // Resolve the type
  auto type = resolve(_g, x);

  // Build the value handle
  auto handle = LLVMConstIntOfStringAndSize(
    type->handle(), x.text.c_str(), x.text.size(), x.base);

  // Build and push the code handle
  _stack.push(std::make_shared<code::Value>(handle, type));
}
