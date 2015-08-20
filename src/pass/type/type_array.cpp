// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/type.hpp"

namespace arrow {
namespace pass {

void Type::visit_type_array(ast::TypeArray& x) {
  // Get the type of the pointee
  auto el_type = Type(_scope).run(*x.element);
  if (!el_type) return;

  // Get the number of elements
  // NOTE: Unsized or dynamic arrays are not yet implemented
  // TODO: Use Fold(X) here
  unsigned size = 0;
  if (x.size.is<ast::Integer>()) {
    size = x.size.as<ast::Integer>()->value();
  } else {
    Log::get().error(
      x.size->span, "array size must be a constant integer.");
  }

  _stack.push_front(new code::TypeArray(el_type, size));
}

}  // namespace pass
}  // namespace arrow
