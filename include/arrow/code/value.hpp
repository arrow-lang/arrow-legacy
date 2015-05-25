// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_CODE_VALUE_H
#define ARROW_CODE_VALUE_H 1

#include <memory>

#include "arrow/llvm.hpp"
#include "arrow/code/item.hpp"
#include "arrow/code/type.hpp"

namespace arrow {

class Generator;

namespace code {

struct Value : Item {
  Value(LLVMValueRef handle, std::shared_ptr<Type> type);

  virtual ~Value() noexcept;

  virtual bool is_value() const noexcept {
    return true;
  }

  virtual bool has_address() const noexcept {
    return false;
  }

  virtual LLVMValueRef value_of(Generator& g) const noexcept;
  virtual LLVMValueRef address_of(Generator& g) const noexcept;

  virtual std::shared_ptr<Type> type() const noexcept;

  std::shared_ptr<code::Value> cast(Generator& g, std::shared_ptr<Type> type);

 private:
  LLVMValueRef _handle;
  std::shared_ptr<Type> _type;
};

}  // namespace code
}  // namespace arrow

#endif  // ARROW_CODE_VALUE_H
