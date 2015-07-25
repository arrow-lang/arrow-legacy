// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void Resolve::visit_call(ast::Call& x) {
  // Resolve the function-type of the operand
  auto type = Resolve(_scope).run(*x.operand);
  if (!type.is<code::TypeFunction>()) return;
  auto function = type.as<code::TypeFunction>();

  // Push the result type of the function
  if (function->result) {
    _stack.push_front(function->result);
  } else {
    _stack.push_front(new code::TypeNone());
  }
}

}  // namespace pass
}  // namespace arrow
