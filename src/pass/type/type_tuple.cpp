// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/type.hpp"

namespace arrow {
namespace pass {

void Type::visit_type_tuple(ast::TypeTuple& x) {
  // Create the initial type IR
  Ref<code::TypeTuple> tuple = new code::TypeTuple();

  // Iterate through the elements; and type each
  tuple->elements.reserve(x.elements.size());
  for (auto& element : x.elements) {
    auto element_type = Type(_scope).run(*element);
    if (!element_type) return;

    tuple->elements.push_back(element_type);
  }

  _stack.push_front(tuple);
}

}  // namespace pass
}  // namespace arrow
