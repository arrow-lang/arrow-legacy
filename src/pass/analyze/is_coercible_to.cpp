// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/analyze.hpp"
#include "arrow/pass/resolve.hpp"
#include "arrow/pass/type.hpp"

namespace arrow {
namespace pass {

bool Analyze::require_is_coercible_to(ast::Node& from, ast::Node& to) {
  if (!is_coercible_to(from, to)) {
    // Resolve the from/to types
    auto from_type = Resolve(_scope).run(from);
    auto to_type = Resolve(_scope).run(to);
    if (!from_type || !to_type) return false;
    if (from_type->is_unknown() || to_type->is_unknown()) return false;

    Log::get().error(to.span,
      "mismatched types: expected `%s`, found `%s`",
      to_type->name().c_str(),
      from_type->name().c_str());

    return false;
  }

  return true;
}

bool Analyze::require_is_coercible_to(ast::Node& from, ast::Type& to) {
  if (!is_coercible_to(from, to)) {
    // Resolve the from/to types
    auto from_type = Resolve(_scope).run(from);
    auto to_type = Type(_scope).run(to);
    if (!from_type || !to_type) return false;
    if (from_type->is_unknown() || to_type->is_unknown()) return false;

    Log::get().error(to.span,
      "mismatched types: expected `%s`, found `%s`",
      to_type->name().c_str(),
      from_type->name().c_str());

    return false;
  }

  return true;
}

bool Analyze::is_coercible_to(ast::Node& from, ast::Node& to) {
  // Resolve the from/to types
  auto from_type = Resolve(_scope).run(from);
  auto to_type = Resolve(_scope).run(to);
  if (!from_type || !to_type) return false;
  if (from_type->is_unknown() || to_type->is_unknown()) return false;

  return is_coercible_to(from_type, to_type);
}

bool Analyze::is_coercible_to(ast::Node& from, ast::Type& to) {
  // Resolve the from/to types
  auto from_type = Resolve(_scope).run(from);
  auto to_type = Type(_scope).run(to);
  if (!from_type || !to_type) return false;
  if (from_type->is_unknown() || to_type->is_unknown()) return false;

  return is_coercible_to(from_type, to_type);
}

bool Analyze::is_coercible_to(Ref<code::Type> from, Ref<code::Type> to) {
  // If the types are equivalent ..
  if (from->equals(*to)) {
    return true;
  }

  // If the types are both sized integers ..
  if (from.is<code::TypeSizedInteger>() &&
      to.is<code::TypeSizedInteger>()) {
    auto from_it = from.as<code::TypeSizedInteger>();
    auto to_it = to.as<code::TypeSizedInteger>();

    // If we are _increasing_ in bit-size ..
    // NOTE: A negative integer literal can never coerce into an
    //       unsigned integer type; this should be checked during a cast.
    if (from_it->bits <= to_it->bits) return true;

    // If we are going from signed to unsigned (and
    // keeping the same bit-size) ..
    if (from_it->is_signed && !to_it->is_signed &&
        from_it->bits == to_it->bits) {
      return true;
    }

    // We cannot go down in bit size and unsigned cannot coerce to signed
    // without a bump in bit size
    return false;
  }

  // If we're going from a sized integer to an unsized integer ..
  if (from.is<code::TypeSizedInteger>() &&
      to.is<code::TypeInteger>()) {
    return true;
  }

  // If we're going from either a sized or unsized integer to a float ..
  if ((from.is<code::TypeSizedInteger>() ||
       from.is<code::TypeInteger>()) &&
      to.is<code::TypeFloat>()) {
    return true;
  }

  // If we're both tuples ..
  if (from.is<code::TypeTuple>() && to.is<code::TypeTuple>()) {
    auto from_t = from.as<code::TypeTuple>();
    auto to_t = to.as<code::TypeTuple>();

    // Of the same length ..
    if (from_t->elements.size() == to_t->elements.size()) {
      // And each element is coercible to the other (in sequence)
      for (unsigned idx = 0; idx < from_t->elements.size(); ++idx) {
        if (!is_coercible_to(
              from_t->elements.at(idx),
              to_t->elements.at(idx))) {
          return false;
        }
      }

      return true;
    }
  }

  return false;
}

}  // namespace pass
}  // namespace arrow
