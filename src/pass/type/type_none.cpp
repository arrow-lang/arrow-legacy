// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/type.hpp"

namespace arrow {
namespace pass {

void Type::visit_type_none(ast::TypeNone&) {
  _stack.push_front(new code::TypeNone());
}

}  // namespace pass
}  // namespace arrow
