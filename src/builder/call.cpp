// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/builder.hpp"
#include "arrow/generator.hpp"
#include "arrow/resolver.hpp"

using arrow::Builder;
using arrow::resolve;

void Builder::visit(ast::Call& x) {
  // TODO: Keyword arguments
  // TODO: Default arguments

  // Attempt to get the function to call
  // TODO: Function "values"
  auto item = build_scalar_of<code::Function>(*x.expression);
  if (!item) { return; }
  auto& type = item->type()->as<code::FunctionType>();

  // Resolve and build each argument (if any)
  // TODO: Check for mis-matched types
  // TODO: Check for too many (or too little) arguments
  std::vector<LLVMValueRef> arguments;
  arguments.reserve(type.parameters.size());
  unsigned arg_index = 0;
  for (auto& arg_node : x.arguments) {
    auto arg = build_scalar_of<code::Value>(*arg_node);
    if (!item) { return; }

    // Cast the argument to the appropriate type
    arg = arg->cast(_g, type.parameters.at(arg_index));

    arguments.push_back(arg->value_of(_g));
    arg_index += 1;
  }

  auto res = LLVMBuildCall(
    _g._irb, item->handle(), arguments.data(), arguments.size(), "");

  _stack.push(std::make_shared<code::Value>(
    res,
    type.result
  ));
}