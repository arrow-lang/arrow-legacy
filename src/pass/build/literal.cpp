// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/build.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void Build::visit_bool(ast::Boolean& x) {
  // Resolve the type of the literal boolean: `bool`
  auto type = Resolve(_scope).run(x);
  if (!type) return;

  // Build the LLVM constant
  auto handle = LLVMConstInt(
    type->handle(), (x.value ? 1 : 0), false);

  // Build and push the value
  _stack.push_front(new code::Value(handle, type));
}

void Build::visit_int(ast::Integer& x) {
  // Resolve the type of the literal integer
  auto type = Resolve(_scope).run(x);
  if (!type) return;

  // Build the LLVM constant
  auto handle = LLVMConstIntOfStringAndSize(
    type->handle(), x.text.c_str(), x.text.size(), 10);

  // Build and push the value
  _stack.push_front(new code::Value(handle, type));
}

void Build::visit_float(ast::Float& x) {
  // Resolve the type of the literal boolean: `float`
  auto type = Resolve(_scope).run(x);
  if (!type) return;

  // Build the LLVM constant
  auto handle = LLVMConstRealOfStringAndSize(
    type->handle(), x.text.c_str(), x.text.size());

  // Build and push the value
  _stack.push_front(new code::Value(handle, type));
}

void Build::visit_none(ast::None&) {
  _stack.push_front(new code::ValueNone());
}

}  // namespace pass
}  // namespace arrow
