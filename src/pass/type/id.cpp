// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/build.hpp"
#include "arrow/pass/expose.hpp"
#include "arrow/pass/analyze.hpp"

namespace arrow {
namespace pass {

void Type::visit_id(ast::Identifier& x) {
  auto item = _scope.get(x.text);
  if (!item) {
    // TODO: Error
    return;
  }

  if (!item->is_type()) {
    // TODO: Error
    return;
  }

  _stack.push_front(item->as<code::Type>());
}

}  // namespace pass
}  // namespace arrow
