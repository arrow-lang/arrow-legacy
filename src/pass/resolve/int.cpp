// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <sstream>

#include "arrow/log.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void Resolve::visit_int(ast::Integer& x) {
  // By default, integer literals are signed
  // By default, integer literals are as big as they need to be to fit
  //  the value (except they'll coerce upwards as needed)
  // NOTE: We add 1 for the sign bit
  auto min = x.minimum_bits() + 1;
  auto bits = 0;

  if (min <= 32) {
    bits = 32;
  } else if (min <= 64) {
    bits = 64;
  } else if (min <= 128) {
    bits = 128;
  } else {
    Log::get().error(
      x.span,
      "integer literal is too large to be represented in any integer type");

    return;
  }

  std::stringstream stream;
  stream << "int" << bits;

  auto item = _scope->get(stream.str()).as<code::Typename>();
  if (!item) return;

  _stack.push_front(item->type);
}

}  // namespace pass
}  // namespace arrow
