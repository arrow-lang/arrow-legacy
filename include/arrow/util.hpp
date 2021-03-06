// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_UTIL_H
#define ARROW_UTIL_H 1

#include "arrow/ast.hpp"
#include "arrow/code.hpp"

namespace arrow {
namespace util {

template <typename T, typename U>
bool is(U& node) {
  return typeid(node) == typeid(T);
}

template <typename T, typename U>
T as(U node) {
  return dynamic_cast<T>(node);
}

/// Determine if an AST node would be considered a constant by LLVM.
extern bool is_static(ast::Node&);

/// Find the nearest enclosing module (of the passed scope).
extern code::Module* current_module(Ref<code::Scope> scope);

/// Cast a `Value` to a `Type`.
Ref<code::Value> cast(
  Compiler::Context& ctx,
  Ref<code::Value> value, ast::Node& node, Ref<code::Type> to_type,
  bool explicit_ = false);

/// Lookup an item from an identifier or path.
extern Ref<code::Item> get_item(Ref<code::Scope>& _scope, ast::Node& node);

/// Check if an item is mutable.
extern bool is_mutable(code::Item& item);

// Get the type from an item
extern Ref<code::Type> type_of(Ref<code::Item> item);

}  // namespace util
}  // namespace arrow

#endif  // ARROW_UTIL_H
