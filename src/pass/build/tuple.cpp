// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/build.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void Build::visit_tuple(ast::Tuple& x) {
  // Resolve the type of the literal boolean: `bool`
  auto type = Resolve(_scope).run(x);
  if (!type) return;

  // Iterate through the elements; and build each
  Ref<code::ValueTuple> res = new code::ValueTuple(type);
  for (auto& element : x.elements) {
    auto el_value = Build(_ctx, _scope).run_scalar(*element);
    if (!el_value) return;

    res->elements.push_back(el_value);
  }

  _stack.push_front(res);
}

}  // namespace pass
}  // namespace arrow
