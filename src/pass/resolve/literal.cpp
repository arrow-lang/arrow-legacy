// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <sstream>

#include "arrow/log.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void Resolve::visit_bool(ast::Boolean&) {
  auto item = _scope->find("bool").as<code::Typename>();
  if (!item) return;

  _stack.push_front(item->type);
}

void Resolve::visit_float(ast::Float&) {
  auto item = _scope->find("float").as<code::Typename>();
  if (!item) return;

  _stack.push_front(item->type);
}

void Resolve::visit_str(ast::String&) {
  auto item = _scope->find("str").as<code::Typename>();
  if (!item) return;

  _stack.push_front(item->type);
}

void Resolve::visit_none(ast::None&) {
  _stack.push_front(new code::TypeNone());
}

}  // namespace pass
}  // namespace arrow
