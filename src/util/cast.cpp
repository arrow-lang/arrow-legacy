// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/util.hpp"

namespace arrow {
namespace util {

Ref<code::Value> cast(
  Compiler::Context& _ctx,
  Ref<code::Value> value, ast::Node& node, Ref<code::Type> to_type,
  bool explicit_
) {
  Ref<code::Type> from_type =
    value->type ? value->type : (new code::TypeNone());

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
  if ((from_type.is<code::TypeSizedInteger>() ||
       from_type.is<code::TypeInteger>() ||
       from_type.is<code::TypeIntegerLiteral>()) &&
      (to_type.is<code::TypeSizedInteger>() ||
       to_type.is<code::TypeInteger>())) {
    unsigned to_bits = 0;
    unsigned from_bits = 0;
    bool is_signed = true;

    if (to_type.is<code::TypeSizedInteger>()) {
      to_bits = to_type.as<code::TypeSizedInteger>()->bits;
    } else {
      // TODO(mehcode): When we add arbitrary percision ..
      to_bits = 128;
    }

    if (from_type.is<code::TypeSizedInteger>()) {
      from_bits = from_type.as<code::TypeSizedInteger>()->bits;
      is_signed = from_type.as<code::TypeSizedInteger>()->is_signed;
    } else if (from_type.is<code::TypeIntegerLiteral>()) {
      is_signed = true;
      explicit_ = true;
      from_bits = 128;
    } else {
      // TODO(mehcode): When we add arbitrary percision ..
      from_bits = 128;
    }

    if (explicit_ || from_type.is<code::TypeSizedInteger>()) {
      if (from_bits <= to_bits) {
        if (is_signed) {
          // Sign-extend
          res = LLVMBuildSExt(
            _ctx.irb, value->get_value(_ctx), to_type->handle(), "");
        } else {
          // Zero-extend
          res = LLVMBuildZExt(
            _ctx.irb, value->get_value(_ctx), to_type->handle(), "");
        }
      } else if (explicit_) {
        // Truncate
        res = LLVMBuildTrunc(
          _ctx.irb, value->get_value(_ctx), to_type->handle(), "");
      }
    }
  }

  // If we're going from an integral type to a float ..
  if (to_type.is<code::TypeFloat>() &&
      (from_type.is<code::TypeSizedInteger>() ||
       from_type.is<code::TypeIntegerLiteral>() ||
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

  // Explicit cast from float to (sized) integer
  if (explicit_ && from_type.is<code::TypeFloat>() &&
      (to_type.is<code::TypeSizedInteger>() ||
       to_type.is<code::TypeIntegerLiteral>() ||
       to_type.is<code::TypeInteger>())) {
    bool is_signed = false;
    if (to_type.is<code::TypeSizedInteger>()) {
      is_signed = to_type.as<code::TypeSizedInteger>()->is_signed;
    } else {
      // TODO(mehcode): When we add arbitrary percision ..
      is_signed = true;
    }

    if (is_signed) {
      // Sign-extend
      res = LLVMBuildFPToSI(
        _ctx.irb, value->get_value(_ctx), to_type->handle(), "");
    } else {
      // Zero-extend
      res = LLVMBuildFPToUI(
        _ctx.irb, value->get_value(_ctx), to_type->handle(), "");
    }
  }

  // Explicit cast from ptr to int
  if (explicit_ && from_type.is<code::TypePointer>() &&
      (to_type.is<code::TypeSizedInteger>() ||
       to_type.is<code::TypeIntegerLiteral>() ||
       to_type.is<code::TypeInteger>())) {
    res = LLVMBuildPtrToInt(
      _ctx.irb, value->get_value(_ctx), to_type->handle(), "");
  }

  // Explicit cast from int to ptr
  if (explicit_ && to_type.is<code::TypePointer>() &&
      (from_type.is<code::TypeSizedInteger>() ||
       from_type.is<code::TypeIntegerLiteral>() ||
       from_type.is<code::TypeInteger>())) {
    res = LLVMBuildIntToPtr(
      _ctx.irb, value->get_value(_ctx), to_type->handle(), "");
  }

  // Explicit cast from ptr to ptr
  if (explicit_ &&
      to_type.is<code::TypePointer>() &&
      from_type.is<code::TypePointer>()) {
    res = LLVMBuildBitCast(
      _ctx.irb, value->get_value(_ctx), to_type->handle(), "");
  }

  // Implicit cast from *mutable T to *T
  if (to_type.is<code::TypePointer>() && from_type.is<code::TypePointer>()) {
    auto from_t = from_type.as<code::TypePointer>();
    auto to_t = to_type.as<code::TypePointer>();
    if (from_t->pointee->equals(*to_t->pointee)) {
      res = value->get_value(_ctx);
    }
  }

  // If we're both tuples ..
  if (from_type.is<code::TypeTuple>() && to_type.is<code::TypeTuple>()) {
    auto from_t = from_type.as<code::TypeTuple>();
    auto to_t = to_type.as<code::TypeTuple>();

    // Of the same length ..
    if (from_t->elements.size() == to_t->elements.size()) {
      // Iterate through the elements; and cast each
      Ref<code::ValueTuple> result = new code::ValueTuple(to_t);
      for (unsigned idx = 0; idx < from_t->elements.size(); ++idx) {
        auto el_value = cast(
          _ctx,
          value->at(_ctx, idx), node, to_t->elements.at(idx), explicit_);
        if (!el_value) return nullptr;

        result->elements.push_back(el_value);
      }

      return result;
    }
  }

  // If we're both arrays ..
  // TODO: Array casting..
  if (from_type.is<code::TypeArray>() && to_type.is<code::TypeArray>()) {
    auto from_t = from_type.as<code::TypeArray>();
    auto to_t = to_type.as<code::TypeArray>();

    // Of the same length ..
    if (from_t->size == to_t->size) {
      // If the from element is an integer literal and the to element
      // is an integer ..
      if (from_t->element.is<code::TypeIntegerLiteral>() &&
          to_t->element.is<code::TypeInteger>()) {
        return value;
      }
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

}  // namespace util
}  // namespace arrow
