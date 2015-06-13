// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/generator.hpp"
#include "arrow/builder.hpp"
// #include "arrow/resolver.hpp"

using arrow::Builder;
// using arrow::resolve;

std::shared_ptr<arrow::code::Item> Builder::do_sequence(
  std::deque<std::shared_ptr<ast::Node>>& seq,
  code::Scope* scope
) {
  std::shared_ptr<code::Item> last;
  bool terminated = false;
  Position unreach_begin{0, 0}, unreach_end{0, 0};
  for (auto& item : seq) {
    bool is_terminated = LLVMGetBasicBlockTerminator(
      LLVMGetInsertBlock(_g._irb));
    if (!terminated && is_terminated) {
      terminated = true;
      unreach_begin = item->span.begin;
    } else if (terminated && !is_terminated) {
      // TODO(mehcode): Log warning of unreachable code
      terminated = false;
    }

    if (!terminated) {
      // Remember the size of the stack at this point (so we can
      // detect if an item gets pushed; and then remove it)
      auto cnt = _stack.size();

      // TODO(mehcode): If statements should have their own scope
      build(*item, scope);

      // Remove anything pushed onto the stack
      for (unsigned i = 0; i < (_stack.size() - cnt); ++i) {
        last = _stack.top();
        _stack.pop();
      }
    } else {
      unreach_end = item->span.end;
    }
  }

  // TODO(mehcode): Log warning about unreachable code block

  return last;
}
