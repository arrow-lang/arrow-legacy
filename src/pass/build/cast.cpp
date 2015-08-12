// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/util.hpp"
#include "arrow/pass/build.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void Build::visit_cast(ast::Cast& x) {
  // Build the operand ..
  auto op = Build(_ctx, _scope).run_scalar(*x.lhs);
  if (!op) return;

  // Resolve the destination type
  auto type = Resolve(_scope).run(x);
  if (!type) return;

  // Cast the RHS to the type of the LHS
  op = util::cast(_ctx, op, *x.lhs, type, true);
  if (!op) return;

  // Push RHS onto the stack
  _stack.push_front(op);
}

}  // namespace pass
}  // namespace arrow
