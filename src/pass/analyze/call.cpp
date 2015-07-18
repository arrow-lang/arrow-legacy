// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/analyze.hpp"

namespace arrow {
namespace pass {

void Analyze::visit_call(ast::Call& x) {
  x.operand->accept(*this);

  for (auto& arg : x.arguments) {
    arg->accept(*this);
  }
}

void Analyze::visit_argument(ast::Argument& x) {
  x.expression->accept(*this);
}

}  // namespace pass
}  // namespace arrow
