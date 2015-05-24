// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/resolver.hpp"
#include "arrow/log.hpp"

using arrow::Resolver;
using arrow::resolve;

void Resolver::visit(ast::Identifier& x) {
  // Get the item in reference
  auto item = _scope.get(x.text);
  if (item == nullptr) {
    Log::get().error(
      x.span, "use of unresolved name `%s`", x.text.c_str());

    return;
  }

  // Determine the type of said item
  if (item->is_type()) {
    // This item -is- a type; easy
    _stack.push(std::static_pointer_cast<code::Type>(item));
  } else if (item->is_value()) {
    // This item -is- a value
    _stack.push(item->as<code::Value>().type());
  } else if (item->is<code::Function>()) {
    // This istem -is- a function
    _stack.push(item->as<code::Function>().type());
  } else {
    Log::get().error(
      x.span, "use of untyped name `%s`", x.text.c_str());

    return;
  }
}
