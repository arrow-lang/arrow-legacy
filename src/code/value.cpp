// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/code/value.hpp"
#include "arrow/generator.hpp"

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
    // TODO: Do the conversion
    return nullptr;
  }
}

auto Value::cast(Generator& g, std::shared_ptr<Type> type) -> std::shared_ptr<Value> {
  // If the types are the same; do nothing
  // TODO: This needs to be extended into a full recursive comparison when
  //  we have generated types (eg. pointers of arbitrary depth)
  auto res = value_of(g);
  if (_type != type) {
    if (_type->is<code::IntegerType>() && type->is<code::IntegerType>()) {
      // Cast between integers
      auto& int_from = _type->as<code::IntegerType>();
      auto& int_to = type->as<code::IntegerType>();

      if (int_from.bits > int_to.bits) {
        // Truncate
        res = LLVMBuildTrunc(g._irb, res, type->handle(), "");
      } else {
        if (int_from.is_signed()) {
          // Sign-extend
          res = LLVMBuildSExt(g._irb, res, type->handle(), "");
        } else {
          // Zero-extend
          res = LLVMBuildZExt(g._irb, res, type->handle(), "");
        }
      }
    }

    if (_type->is<code::IntegerType>() && type->is<code::FloatType>()) {
      // Convert integer to float
      if (_type->is_signed()) {
        res = LLVMBuildSIToFP(g._irb, res, type->handle(), "");
      } else {
        res = LLVMBuildUIToFP(g._irb, res, type->handle(), "");
      }
    }

    if (type->is<code::IntegerType>() && _type->is<code::FloatType>()) {
      // Convert float to integer
      if (type->is_signed()) {
        res = LLVMBuildFPToSI(g._irb, res, type->handle(), "");
      } else {
        res = LLVMBuildFPToUI(g._irb, res, type->handle(), "");
      }
    }

    if (type->is<code::FloatType>() && _type->is<code::FloatType>()) {
      // Cast between floats
      auto& float_from = _type->as<code::FloatType>();
      auto& float_to = type->as<code::FloatType>();

      if (float_from.bits > float_to.bits) {
        // Truncate
        res = LLVMBuildFPTrunc(g._irb, res, type->handle(), "");
      } else {
        // Extend
        res = LLVMBuildFPExt(g._irb, res, type->handle(), "");
      }
    }
  }

  // Return a new, casted value
  return std::make_shared<code::Value>(res, type);
}
