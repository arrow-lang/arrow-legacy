// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/builder.hpp"
#include "arrow/resolver.hpp"

using arrow::Builder;
using arrow::resolve;

void Builder::visit_block(ast::Block& x) {
  // Create a new scope for this block
  // TODO: This is a massive fail waiting to happen -- need to use shared_ptr
  code::Scope nested("", _cs);

  // Iterate and build each node in the block sequence
  for (auto& el : x.sequence) {
    build(*el, &nested);
  }
}
