// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void Resolve::visit_conditional(ast::Conditional& x) {
  // Resolve the types of the true and false blocks
  auto lhs_type = Resolve(_scope).run(*x.lhs);
  auto rhs_type = Resolve(_scope).run(*x.rhs);
  if (!lhs_type || !rhs_type) return;
  if (lhs_type->is_unknown() || rhs_type->is_unknown()) return;

  // Intersect the matched types ..
  auto final_type = code::intersect_all({lhs_type, rhs_type});
  if (final_type) {
    _stack.push_front(final_type);
  }
}

}  // namespace pass
}  // namespace arrow
