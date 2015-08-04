// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/util.hpp"

namespace arrow {
namespace util {

bool is_static(ast::Node& node) {
  // Literal: String, Float, Integer, Boolean, None
  // TODO(_): When we have string interpolation a string may not be static
  if (is<ast::String>(node) ||
      is<ast::Float>(node) ||
      is<ast::Integer>(node) ||
      is<ast::None>(node) ||
      is<ast::Boolean>(node)) {
    return true;
  }

  // Tuple
  if (is<ast::Tuple>(node)) {
    auto tuple = as<ast::Tuple*>(&node);
    for (auto& element : tuple->elements) {
      if (!is_static(*element)) return false;
    }
  }

  return false;
}

}  // namespace util
}  // namespace arrow
