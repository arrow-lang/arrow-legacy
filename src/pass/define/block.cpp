// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/define.hpp"

namespace arrow {
namespace pass {

void Define::visit_block(ast::Block& x) {
  // Enter the <anonymous> scope block ..
  _scope->enter(&x);

  // Run the base method (which iterates over the statements)
  Visitor::visit_block(x);

  // Exit the <anonymous> scope block
  _scope->exit();
}

}  // namespace pass
}  // namespace arrow
