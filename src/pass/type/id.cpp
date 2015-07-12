// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/type.hpp"

namespace arrow {
namespace pass {

void Type::visit_id(ast::Identifier& x) {
  auto item = _scope->get(x.text);
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
