// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/analyze.hpp"

namespace arrow {
namespace pass {

void Analyze::visit_id(ast::Identifier& x) {
  // Check for a declared name
  auto ref = _x_name.find(x.text);
  if (ref == _x_name.end()) {
    Log::get().error(
      x.span, "use of unresolved name '%s'", x.text.c_str());

    return;
  }

  // TODO(mehcode): Check for at least one definite assignment
}

}  // namespace pass
}  // namespace arrow
