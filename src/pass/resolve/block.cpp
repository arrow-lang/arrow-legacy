// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void Resolve::visit_block(ast::Block& x) {
  // Enter the <anonymous> scope block ..
  _scope->enter(&x);

  if (x.statements.size() != 0) {
    // Resolve (and push) the type of the last statement in the block
    auto type = Resolve(_scope).run(
      *(x.statements.at(x.statements.size() - 1)));

    if (type) {
      _stack.push_front(type);
    }
  }

  // Exit the <anonymous> scope block
  _scope->exit();
}

}  // namespace pass
}  // namespace arrow
