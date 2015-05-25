// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/generator.hpp"
#include "arrow/builder.hpp"
#include "arrow/resolver.hpp"

using arrow::Builder;
using arrow::resolve;

void Builder::visit(ast::String& x) {
  // TODO: Unescape escape sequences

  auto type = resolve(_g, *_cs, x);
  auto handle = LLVMBuildGlobalStringPtr(_g._irb, x.text().c_str(), "");

  _stack.push(std::make_shared<code::Value>(handle, type));
}
