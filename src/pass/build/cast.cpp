// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/build.hpp"

namespace arrow {
namespace pass {

Ref<code::Value> Build::do_cast(
  Ref<code::Value> value, ast::Node& node, Ref<code::Type> to_type,
  bool explicit_
) {
  auto from_type = value->type;
  LLVMValueRef res = nullptr;

  // If the to-type is `any` ..
  if (to_type.is<code::TypeAny>()) {
    return value;
  }

  // If the types are equivalent ..
  if (from_type->equals(*to_type)) {
    return value;
  }

  // If the types are both sized integers ..
  // FIXME: A negative integer literal can never coerce into an
  //       unsigned integer type; this should be checked during a cast.
  if (from_type.is<code::TypeSizedInteger>() &&
      (to_type.is<code::TypeSizedInteger>() ||
       to_type.is<code::TypeInteger>())) {
    auto from_it = from_type.as<code::TypeSizedInteger>();
    unsigned to_bits = 0;
    if (to_type.is<code::TypeSizedInteger>()) {
      to_bits = to_type.as<code::TypeSizedInteger>()->bits;
    } else {
      // TODO(mehcode): When we add arbitrary percision ..
      to_bits = 128;
    }

    if (from_it->bits <= to_bits) {
      if (from_it->is_signed) {
        // Sign-extend
        res = LLVMBuildSExt(
          _ctx.irb, value->get_value(_ctx), to_type->handle(), "");
      } else {
        // Zero-extend
        res = LLVMBuildZExt(
          _ctx.irb, value->get_value(_ctx), to_type->handle(), "");
      }
    }
  }

  // TODO(mehcode): Explicit cast to truncate integers

  // If we're going from an integral type to a float ..
  if (to_type.is<code::TypeFloat>() &&
      (from_type.is<code::TypeSizedInteger>() ||
       from_type.is<code::TypeInteger>())) {
    bool from_is_signed = false;
    if (from_type.is<code::TypeSizedInteger>()) {
      from_is_signed = from_type.as<code::TypeSizedInteger>()->is_signed;
    } else {
      // TODO(mehcode): When we add arbitrary percision ..
      from_is_signed = true;
    }

    if (from_is_signed) {
      // Sign-extend
      res = LLVMBuildSIToFP(
        _ctx.irb, value->get_value(_ctx), to_type->handle(), "");
    } else {
      // Zero-extend
      res = LLVMBuildUIToFP(
        _ctx.irb, value->get_value(_ctx), to_type->handle(), "");
    }
  }

  // TODO(mehcode): Explicit cast from float to (sized) integer
  // TODO(mehcode): Explicit cast from ptr to int and int to ptr

  // If we're both tuples ..
  if (from_type.is<code::TypeTuple>() && to_type.is<code::TypeTuple>()) {
    auto from_t = from_type.as<code::TypeTuple>();
    auto to_t = to_type.as<code::TypeTuple>();

    // Of the same length ..
    if (from_t->elements.size() == to_t->elements.size()) {
      // Iterate through the elements; and cast each
      Ref<code::ValueTuple> result = new code::ValueTuple(to_t);
      for (unsigned idx = 0; idx < from_t->elements.size(); ++idx) {
        auto el_value = do_cast(
          value->at(_ctx, idx), node, to_t->elements.at(idx), explicit_);
        if (!el_value) return nullptr;

        result->elements.push_back(el_value);
      }

      return result;
    }
  }

  // If we didn't manage to cast the expression
  if (!res) {
    // FIXME: Get location from AST
    Log::get().error(node.span, "no %s conversion from '%s' to '%s'",
      explicit_ ? "explicit" : "implicit",
      from_type->name().c_str(),
      to_type->name().c_str());

    return nullptr;
  }

  // Return a new, casted value
  return new code::Value(res, to_type);
}

// void Build::visit_cast(ast::Cast& x) {
// }

}  // namespace pass
}  // namespace arrow
