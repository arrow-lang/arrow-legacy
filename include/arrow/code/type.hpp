// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_CODE_TYPE_H
#define ARROW_CODE_TYPE_H 1

#include <deque>
#include <memory>

#include "arrow/llvm.hpp"
#include "arrow/code/item.hpp"

namespace arrow {
namespace code {

struct Type : Item {
  virtual ~Type() noexcept;

  virtual LLVMTypeRef handle() const noexcept = 0;

  virtual bool is_type() const noexcept {
    return true;
  }

  virtual bool is_signed() const noexcept {
    return false;
  }
};

struct IntegerType : Type {
  IntegerType(unsigned bits, bool is_signed);

  virtual ~IntegerType() noexcept;

  virtual LLVMTypeRef handle() const noexcept;

  virtual bool is_signed() const noexcept {
    return _is_signed;
  }

  unsigned bits;

 private:
  bool _is_signed;
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

struct StringType : Type {
  virtual ~StringType() noexcept;

  virtual LLVMTypeRef handle() const noexcept;
};

struct FunctionType : Type {
  explicit FunctionType(std::shared_ptr<code::Type> result);

  virtual ~FunctionType() noexcept;

  virtual LLVMTypeRef handle() const noexcept;

  std::shared_ptr<Type> result;
  std::deque<std::shared_ptr<Type>> parameters;
};

}  // namespace code
}  // namespace arrow

#endif  // ARROW_CODE_TYPE_H
