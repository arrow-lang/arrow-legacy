// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <sstream>

#include "arrow/log.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void Resolve::visit_int(ast::Integer&) {
  // By default, integer literals are `int`
  _stack.push_front(new code::TypeIntegerLiteral());
}

}  // namespace pass
}  // namespace arrow
