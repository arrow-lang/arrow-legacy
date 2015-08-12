// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/util.hpp"
#include "arrow/pass/resolve.hpp"
#include "arrow/pass/type.hpp"

namespace arrow {
namespace pass {

void Resolve::visit_cast(ast::Cast& x) {
  // Resolve the destination type
  auto type = Type(_scope).run(*x.rhs);
  if (!type) return;

  // Push RHS onto the stack
  _stack.push_front(type);
}

}  // namespace pass
}  // namespace arrow
