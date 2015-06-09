// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/builder.hpp"
#include "arrow/resolver.hpp"

using arrow::Builder;
using arrow::resolve;

void Builder::visit_bool(ast::Boolean& x) {
  auto type = resolve(_g, *_cs, x);
  auto handle = LLVMConstInt(type->handle(_g), x.value ? 1 : 0, 0);

  _stack.push(std::make_shared<code::Value>(&x, _cs, handle, type));
}
