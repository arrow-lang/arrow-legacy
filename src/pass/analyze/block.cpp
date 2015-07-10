// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/analyze.hpp"

namespace arrow {
namespace pass {

void Analyze::visit_block(ast::Block& x) {
  for (auto& stmt : x.statements) {
    stmt->accept(*this);
  }
}

void Analyze::visit_loop(ast::Loop& x) {
  if (x.condition) {
    x.condition->accept(*this);
  }

  x.block->accept(*this);
}

void Analyze::visit_select(ast::Select& x) {
  for (auto& br : x.branches) {
    br->accept(*this);
  }

  if (x.else_block) {
    x.else_block->accept(*this);
  }
}

void Analyze::visit_select_branch(ast::SelectBranch& x) {
  x.condition->accept(*this);
  x.block->accept(*this);
}

}  // namespace pass
}  // namespace arrow
