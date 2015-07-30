// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_CODE_CONTAINER_H
#define ARROW_CODE_CONTAINER_H 1

#include "arrow/code/scope.hpp"

namespace arrow {
namespace code {

struct Container {
  Container(std::string name, Ref<code::Scope> parent_scope)
    : scope(new Scope(name, parent_scope, this)) {
  }

  virtual ~Container() noexcept;

  /// Container scope
  Ref<code::Scope> scope;
};

}  // namespace code
}  // namespace arrow

#endif  // ARROW_CODE_CONTAINER_H
