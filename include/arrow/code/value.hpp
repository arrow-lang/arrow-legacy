// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_CODE_VALUE_H
#define ARROW_CODE_VALUE_H 1

#include <memory>

#include "arrow/code/item.hpp"
#include "arrow/ast/nodes.hpp"
#include "arrow/code/type.hpp"
#include "arrow/llvm.hpp"

namespace arrow {

class Generator;

namespace code {

class Scope;

struct Value : Item {
  Value(ast::Node* context,
        code::Scope* scope,
        LLVMValueRef handle, std::shared_ptr<Type> type,
        bool _mutable = false,
        bool _address = false);

  virtual ~Value() noexcept;

  virtual bool is_value() const noexcept {
    return true;
  }

  virtual bool is_mutable() const noexcept {
    return _mutable;
  }

  virtual bool has_address() const noexcept {
    return _address;
  }

  virtual LLVMValueRef value_of(Generator& g) const noexcept;
  virtual LLVMValueRef address_of(Generator& g) const noexcept;

  virtual std::shared_ptr<Type> type() const noexcept;

  std::shared_ptr<code::Value> cast(
    Generator& g, ast::Node& ctx, std::shared_ptr<Type> type,
    bool explicit_ = false);

 private:
  LLVMValueRef _handle;
  std::shared_ptr<Type> _type;
  bool _mutable;
  bool _address;
};

}  // namespace code
}  // namespace arrow

#endif  // ARROW_CODE_VALUE_H
