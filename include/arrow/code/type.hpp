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
  virtual LLVMTypeRef handle() = 0;
};

struct TypeBoolean : Type {
  virtual ~TypeBoolean() noexcept;

  virtual LLVMTypeRef handle();
};

struct TypeFloat : Type {
  virtual ~TypeFloat() noexcept;

  virtual LLVMTypeRef handle();
};

struct TypeInteger : Type {
  explicit TypeInteger(unsigned bits, bool is_signed = true)
    : bits(bits), is_signed(is_signed) {
  }

  virtual ~TypeInteger() noexcept;

  virtual LLVMTypeRef handle();

  unsigned bits;
  bool is_signed;
};

struct TypeTuple : Type {
  virtual ~TypeTuple() noexcept;

  virtual LLVMTypeRef handle();

  std::vector<Ref<code::Type>> elements;

 private:
  LLVMTypeRef _handle;
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
