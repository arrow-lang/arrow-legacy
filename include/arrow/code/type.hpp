// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_CODE_TYPE_H
#define ARROW_CODE_TYPE_H 1

#include "arrow/llvm.hpp"
#include "arrow/code/item.hpp"

namespace arrow {
namespace code {

struct Type {
  virtual ~Type() noexcept;

  /// Get the LLVM type handle.
  virtual LLVMTypeRef handle() const = 0;
};

struct BooleanType : Type {
  virtual ~BooleanType() noexcept;

  virtual LLVMTypeRef handle() const;
};

struct FloatType : Type {
  virtual ~FloatType() noexcept;

  virtual LLVMTypeRef handle() const;
};

struct IntegerType : Type {
  explicit IntegerType(unsigned bits) : bits(bits) {
  }

  virtual ~IntegerType() noexcept;

  virtual LLVMTypeRef handle() const;

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
