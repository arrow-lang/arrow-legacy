// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void Resolve::visit_array(ast::Array& x) {
  // Iterate through the elements; and type each
  std::vector<Ref<code::Type>> elements;
  elements.reserve(x.elements.size());
  for (auto& element : x.elements) {
    auto element_type = Resolve(_scope).run(*element);
    if (!element_type) return;

    elements.push_back(element_type);
  }

  // Intersect all the types
  auto final_type = code::intersect_all(elements);
  if (!final_type) {
    Log::get().error(
      x.span, "cannot resolve a common type from the array elements");

    return;
  }

  _stack.push_front(new code::TypeArray(final_type, x.elements.size()));
}

void Resolve::visit_index(ast::Index& x) {
  // Resolve the type of the LHS
  auto lhs = Resolve(_scope).run(*x.lhs);
  if (!lhs) return;
  if (lhs->is_unknown()) {
    _stack.push_front(new code::TypeUnknown());
    return;
  }

  // Check if we are an array
  if (!lhs.is<code::TypeArray>()) {
    Log::get().error(
      x.span, "cannot apply operator `[]` to type '%s'",
      lhs->name().c_str());

    return;
  }

  // Push the element type of the array
  _stack.push_front(lhs.as<code::TypeArray>()->element);
}

}  // namespace pass
}  // namespace arrow
