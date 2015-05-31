// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/code/value.hpp"
#include "arrow/generator.hpp"
#include "arrow/log.hpp"

using arrow::code::Value;

Value::Value(LLVMValueRef handle, std::shared_ptr<Type> type)
  : _handle{handle}, _type{type} {
}

Value::~Value() noexcept {
}

auto Value::type() const noexcept -> std::shared_ptr<Type> {
  return _type;
}

LLVMValueRef Value::value_of(Generator& g) const noexcept {
  if (has_address()) {
    return LLVMBuildLoad(g._irb, _handle, "");
  } else {
    return _handle;
  }
}

LLVMValueRef Value::address_of(Generator&) const noexcept {
  if (has_address()) {
    return _handle;
  } else {
    // TODO(mehcode): Do the conversion
    return nullptr;
  }
}

auto Value::cast(Generator& g, ast::Node& ctx, std::shared_ptr<Type> type)
  -> std::shared_ptr<Value> {
  auto value = value_of(g);
  decltype(value) res = nullptr;

  if (_type->equals(*type)) {
    return std::make_shared<code::Value>(value, type);
  }

  if (_type->is<code::IntegerType>() && type->is<code::IntegerType>()) {
    // Cast between integers
    auto& int_from = _type->as<code::IntegerType>();
    auto& int_to = type->as<code::IntegerType>();

    if (int_from.bits <= int_to.bits) {
      if (int_from.is_signed()) {
        // Sign-extend
        res = LLVMBuildSExt(g._irb, value, type->handle(), "");
      } else {
        // Zero-extend
        res = LLVMBuildZExt(g._irb, value, type->handle(), "");
      }
    } else if (ctx.is<ast::Integer>()) {
      // This is a constant integer
      auto bits = ctx.as<ast::Integer>().minimum_bits();
      if (bits <= int_to.bits) {
        // Truncate (but not really; this is a bitcast)
        res = LLVMBuildTrunc(g._irb, value, type->handle(), "");
      } else {
        Log::get().error(ctx.span,
          "integer literal out of range for '%s'", type->name().c_str());

        return nullptr;
      }
    }
  }

  if (_type->is<code::IntegerType>() && type->is<code::FloatType>()) {
    // Convert integer to float
    if (_type->is_signed()) {
      res = LLVMBuildSIToFP(g._irb, value, type->handle(), "");
    } else {
      res = LLVMBuildUIToFP(g._irb, value, type->handle(), "");
    }
  }

  if (type->is<code::IntegerType>() && _type->is<code::FloatType>()) {
    // Convert float to integer
    if (type->is_signed()) {
      res = LLVMBuildFPToSI(g._irb, value, type->handle(), "");
    } else {
      res = LLVMBuildFPToUI(g._irb, value, type->handle(), "");
    }
  }

  if (type->is<code::FloatType>() && _type->is<code::FloatType>()) {
    // Cast between floats
    auto& float_from = _type->as<code::FloatType>();
    auto& float_to = type->as<code::FloatType>();

    if (float_from.bits > float_to.bits) {
      // Truncate
      res = LLVMBuildFPTrunc(g._irb, value, type->handle(), "");
    } else {
      // Extend
      res = LLVMBuildFPExt(g._irb, value, type->handle(), "");
    }
  }

  // If we didn't manage to cast the expression
  if (!res) {
    // FIXME: Get location from AST
    auto from_name = _type->name();
    auto to_name = type->name();
    Log::get().error(ctx.span, "no implicit conversion from '%s' to '%s'",
      from_name.c_str(), to_name.c_str());

    return nullptr;
  }

  // Return a new, casted value
  return std::make_shared<code::Value>(res, type);
}
