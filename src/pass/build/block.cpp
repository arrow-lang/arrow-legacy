// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/build.hpp"

namespace arrow {
namespace pass {

void Build::visit_block(ast::Block& x) {
  // Enter the <anonymous> scope block ..
  _scope->enter(&x);

  Ref<code::Value> last_value;
  bool terminated = false;
  Position unreach_begin{0, 0}, unreach_end{0, 0};
  for (auto& node : x.statements) {
    bool is_terminated = LLVMGetBasicBlockTerminator(
      LLVMGetInsertBlock(_ctx.irb));

    if (!terminated && is_terminated) {
      terminated = true;
      unreach_begin = node->span.begin;
    } else if (terminated && !is_terminated) {
      // TODO(mehcode): Log warning of unreachable code
      terminated = false;
    }

    if (!terminated) {
      // Remember the size of the stack at this point (so we can
      // detect if an item gets pushed; and then remove it)
      auto cnt = _stack.size();

      // Accept the statement ..
      node->accept(*this);

      // Remove anything pushed onto the stack
      for (unsigned i = 0; i < (_stack.size() - cnt); ++i) {
        last_value = _stack.front();
        _stack.pop_front();
      }
    } else {
      unreach_end = node->span.end;
    }
  }

  // TODO(mehcode): Log warning about unreachable code block

  if (!terminated && last_value) {
    // Push the last value as the value of this block
    _stack.push_front(last_value);
  }

  // Exit the <anonymous> scope block
  _scope->exit();
}

}  // namespace pass
}  // namespace arrow
