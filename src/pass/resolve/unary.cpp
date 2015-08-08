// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <sstream>

#include "arrow/match.hpp"
#include "arrow/log.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void Resolve::do_unary(
  ast::Unary& x, std::function<Ref<code::Type>(Ref<code::Type>)> cb
) {
  // Resolve the type of the operand expression
  auto type = Resolve(_scope).run(*x.operand);
  if (!type) return;

  if (type->is_unknown()) {
    _stack.push_front(type);
    return;
  }

  // Determine the resultant type
  auto result = cb(type);
  if (!result) {
    // Report operator not-found
    Log::get().error(x.span, "cannot apply unary operator `%s` to type '%s'",
      x.punctuator(),
      type->name().c_str());

    return;
  }

  _stack.push_front(result);
}

void Resolve::visit_identity(ast::Identity& x) {
  do_unary(x, [&, this](Ref<code::Type> type) -> Ref<code::Type> {
    if (type.is<code::TypeInteger>() ||
        type.is<code::TypeFloat>() ||
        type.is<code::TypeSizedInteger>()) {
      return type;
    }

    return nullptr;
  });
}

void Resolve::visit_negate(ast::Negate& x) {
  do_unary(x, [&, this](Ref<code::Type> type) -> Ref<code::Type> {
    if (type.is<code::TypeInteger>() ||
        type.is<code::TypeFloat>() ||
        type.is<code::TypeSizedInteger>()) {
      return type;
    }

    return nullptr;
  });
}

void Resolve::visit_bit_not(ast::BitNot& x) {
  do_unary(x, [&, this](Ref<code::Type> type) -> Ref<code::Type> {
    if (type.is<code::TypeInteger>() ||
        type.is<code::TypeBoolean>() ||
        type.is<code::TypeSizedInteger>()) {
      return type;
    }

    return nullptr;
  });
}

void Resolve::visit_not(ast::Not& x) {
  do_unary(x, [&, this](Ref<code::Type> type) -> Ref<code::Type> {
    if (type.is<code::TypeBoolean>()) {
      return type;
    }

    return nullptr;
  });
}

}  // namespace pass
}  // namespace arrow
