// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/type.hpp"

namespace arrow {
namespace pass {

void Type::visit_type_path(ast::TypePath& x) {
  // TODO(mehcode): Support a longer than 1 typepath
  auto item = _scope->find(x.segments[0]);
  if (!item) {
    Log::get().error(
      x.span, "use of unresolved name '%s'", x.segments[0].c_str());

    return;
  }

  // Pull the referred type out of the typename (and push to the stack)
  if (item.is<code::Typename>()) {
    _stack.push_front(item.as<code::Typename>()->type);
  } else if (item.is<code::Structure>()) {
    _stack.push_front(item.as<code::Structure>()->type);
  }
}

}  // namespace pass
}  // namespace arrow
