// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_CODE_TYPE_H
#define ARROW_CODE_TYPE_H 1

#include "arrow/code/item.hpp"

namespace arrow {
namespace code {

struct Type {
  virtual ~Type() noexcept;

  /// Get the LLVM type handle.
  LLVMTypeRef handle() const = 0;
};

struct BooleanType : Type {
  virtual ~BooleanType() noexcept;

  LLVMTypeRef handle() const;
};

struct FloatType : Type {
  FloatType(unsigned bits) : bits(bits) {
  }

  virtual ~FloatType() noexcept;

  LLVMTypeRef handle() const;

  unsigned bits;
};

struct IntegerType : Type {
  IntegerType(unsigned bits) : bits(bits) {
  }

  virtual ~IntegerType() noexcept;

  LLVMTypeRef handle() const;

  unsigned bits;
};

struct Typename : Item {
  Typename(ast::Node* context, std::string name, Ref<code::Type> type)
    : Item(context, name), type(type) {
  }

  virtual ~Typename() noexcept;

  Ref<Type> type;
};

}  // namespace code
}  // namespace arrow

#endif  // ARROW_CODE_TYPE_H
