// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_CODE_TYPE_H
#define ARROW_CODE_TYPE_H 1

#include "arrow/llvm.hpp"
#include "arrow/code/item.hpp"

namespace arrow {
namespace code {

struct Type : Item {
  virtual ~Type() noexcept;

  virtual LLVMTypeRef handle() const noexcept = 0;

  template <typename T>
  bool is() {
    return dynamic_cast<T*>(this) != nullptr;
  }

  template <typename T>
  T& as() {
    return *(dynamic_cast<T*>(this));
  }

  virtual bool is_type() const noexcept {
    return true;
  }
};

struct IntegerType : Type {
  IntegerType(unsigned bits, bool is_signed);

  virtual ~IntegerType() noexcept;

  virtual LLVMTypeRef handle() const noexcept;

  unsigned bits;
  bool is_signed;
};

struct BooleanType : Type {
  virtual ~BooleanType() noexcept;

  virtual LLVMTypeRef handle() const noexcept;
};

struct FloatType : Type {
  explicit FloatType(unsigned bits);

  virtual ~FloatType() noexcept;

  virtual LLVMTypeRef handle() const noexcept;

  unsigned bits;
};

}  // namespace code
}  // namespace arrow

#endif  // ARROW_CODE_TYPE_H
