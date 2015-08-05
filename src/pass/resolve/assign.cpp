// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void Resolve::visit_assign(ast::Assign& x) {
  // Resolve the operand type of the assignment
  auto type = Resolve(_scope).run(*x.lhs);
  if (!type) return;

  // Push the type of the expression
  if (type) {
    _stack.push_front(type);
  }
}

}  // namespace pass
}  // namespace arrow
