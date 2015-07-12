// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/analyze.hpp"

namespace arrow {
namespace pass {

void Analyze::visit_assign(ast::Assign&) {
  // TODO(mehcode): Expand pattern
  std::printf("ASSIGN: ?\n");
}

}  // namespace pass
}  // namespace arrow
