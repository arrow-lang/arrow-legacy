// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_CODE_VALUE_H
#define ARROW_CODE_VALUE_H 1

#include "arrow/code/type.hpp"
#include "arrow/llvm.hpp"
#include "arrow/compiler.hpp"

namespace arrow {
namespace code {

struct Value {
  Value(LLVMValueRef handle, Ref<code::Type> type)
    : type(type), _handle(handle) {
  }

  virtual ~Value() noexcept;

  virtual LLVMValueRef get_address(Compiler::Context& ctx);
  virtual LLVMValueRef get_value(Compiler::Context& ctx);

  virtual bool has_address() const {
    auto type_handle = LLVMTypeOf(_handle);

    if (type.is<TypeString>()) {
      type_handle = LLVMGetElementType(type_handle);
    }

    if ((typeid(*this) == typeid(Value)) && type.is<TypePointer>()) {
      auto recur_type = type;
      while (recur_type.is<TypePointer>()) {
        type_handle = LLVMGetElementType(type_handle);
        recur_type = recur_type.as<TypePointer>()->pointee;
      }

      if (recur_type.is<TypeString>()) {
        type_handle = LLVMGetElementType(type_handle);
      }
    }

    return LLVMGetTypeKind(type_handle) == LLVMPointerTypeKind;
  }

  virtual bool is_assignable() const {
    return has_address();
  }

  virtual Ref<code::Value> at(Compiler::Context& ctx, unsigned index);

  Ref<code::Type> type;

 protected:
  LLVMValueRef _handle;
};

struct ValueNone : Value {
  ValueNone()
    : Value(nullptr, new code::TypeNone()) {
  }

  virtual ~ValueNone() noexcept;
};

struct ValueTuple : Value {
  explicit ValueTuple(Ref<TypeTuple> type)
    : Value(nullptr, type), elements() {
  }

  virtual ~ValueTuple() noexcept;

  virtual LLVMValueRef get_value(Compiler::Context& ctx);

  /// Check if all elements have addresses or are tuples and assignable
  virtual bool is_assignable() const;

  virtual bool has_address() const {
    return false;
  }

  virtual Ref<code::Value> at(Compiler::Context& ctx, unsigned index);

  /// Ordered sequence of elements (in the literal tuple)
  std::deque<Ref<Value>> elements;
};

}  // namespace code
}  // namespace arrow

#endif  // ARROW_CODE_VALUE_H
