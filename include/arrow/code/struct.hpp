// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_CODE_STRUCT_H
#define ARROW_CODE_STRUCT_H 1

#include "arrow/llvm.hpp"
#include "arrow/code/item.hpp"
#include "arrow/code/container.hpp"
#include "arrow/code/value.hpp"

namespace arrow {
namespace code {

struct TypeStructure;

struct Structure : Item {
  Structure(ast::Node* context, std::string name)
    : Item(context, name), type(nullptr) {
  }

  virtual ~Structure() noexcept;

  Ref<TypeStructure> type;
};

}  // namespace code
}  // namespace arrow

#endif  // ARROW_CODE_STRUCT_H
