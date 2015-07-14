// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/build.hpp"

namespace arrow {
namespace pass {

void Build::visit_id(ast::Identifier& x) {
  auto item = _scope->get(x.text);
  if (item == nullptr) {
    Log::get().error(
      x.span, "use of unresolved name '%s'", x.text.c_str());

    return;
  }

  _stack.push_front(item);
}

}  // namespace pass
}  // namespace arrow
