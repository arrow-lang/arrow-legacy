// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/type.hpp"

namespace arrow {
namespace pass {

void Type::visit_type_pointer(ast::TypePointer& x) {
  // Get the type of the pointee
  auto pointee_type = Type(_scope).run(*x.pointee);
  if (!pointee_type) return;

  _stack.push_front(new code::TypePointer(pointee_type, x.is_mutable));
}

}  // namespace pass
}  // namespace arrow
