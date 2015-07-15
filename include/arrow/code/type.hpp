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

  virtual bool equals(Type& other) const {
    return typeid(other) == typeid(*this);
  }

  virtual std::string name() const = 0;

  /// Get the LLVM type handle.
  virtual LLVMTypeRef handle() = 0;
};

struct TypeBoolean : Type {
  virtual ~TypeBoolean() noexcept;

  virtual std::string name() const {
    return "bool";
  }

  virtual LLVMTypeRef handle();
};

struct TypeFloat : Type {
  virtual ~TypeFloat() noexcept;

  virtual std::string name() const {
    return "float";
  }

  virtual LLVMTypeRef handle();
};

struct TypeByte : Type {
  virtual ~TypeByte() noexcept;

  virtual std::string name() const {
    return "byte";
  }

  virtual LLVMTypeRef handle();
};

struct TypeInteger : Type {
  virtual ~TypeInteger() noexcept;

  virtual LLVMTypeRef handle();

  virtual std::string name() const {
    return "int";
  }
};

struct TypeSizedInteger : Type {
  explicit TypeSizedInteger(unsigned bits, bool is_signed = true)
    : bits(bits), is_signed(is_signed) {
  }

  virtual ~TypeSizedInteger() noexcept;

  virtual bool equals(Type& other) const;

  virtual LLVMTypeRef handle();

  virtual std::string name() const;

  unsigned bits;
  bool is_signed;
};

struct TypeTuple : Type {
  virtual ~TypeTuple() noexcept;

  virtual LLVMTypeRef handle();

  virtual bool equals(Type& other) const;

  virtual std::string name() const;

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
