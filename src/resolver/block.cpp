// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/resolver.hpp"

using arrow::Resolver;
using arrow::resolve;

void Resolver::visit_block(ast::Block& x) {
  if (x.sequence.size() == 0) {
    // FIXME: Nothing in block (error?)
    return;
  }

  // Resolve the final statement (ignore all others)
  _stack.push(resolve(_g, _scope, *(x.sequence.front())));
}
