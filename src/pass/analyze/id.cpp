// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/analyze.hpp"

namespace arrow {
namespace pass {

void Analyze::visit_id(ast::Identifier& x) {
  std::printf("USE: %s\n", x.text.c_str());
}

}  // namespace pass
}  // namespace arrow
