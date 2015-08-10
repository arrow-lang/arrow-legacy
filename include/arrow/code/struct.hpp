// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_CODE_FUNCTION_H
#define ARROW_CODE_FUNCTION_H 1

#include "arrow/llvm.hpp"
#include "arrow/code/item.hpp"
#include "arrow/code/container.hpp"
#include "arrow/code/value.hpp"

namespace arrow {
namespace code {

struct Structure : Item {
  Structure(ast::Node* context, std::string name)
    : Item(context, name) {
  }

  virtual ~Structure() noexcept;
};

}  // namespace code
}  // namespace arrow

#endif  // ARROW_CODE_FUNCTION_H
