// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/build.hpp"

namespace arrow {
namespace pass {

void Build::visit_block(ast::Block& x) {
  for (auto& node : x.statements) {
    node->accept(*this);
  }
}

}  // namespace pass
}  // namespace arrow
