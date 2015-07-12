// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/type.hpp"

namespace arrow {
namespace pass {

void Type::visit_type_path(ast::TypePath& x) {
  // TODO(mehcode): Support a longer than 1 typepath
  auto item = _scope->get(x.segments[0]);
  if (!item) {
    // TODO(mehcode): Error
    return;
  }

  if (!item.is<code::Typename>()) {
    // TODO(mehcode): Error
    return;
  }

  // Pull the referred type out of the typename (and push to the stack)
  auto typename_ = item.as<code::Typename>();
  _stack.push_front(typename_->type);
}

}  // namespace pass
}  // namespace arrow
