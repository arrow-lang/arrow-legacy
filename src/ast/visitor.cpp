// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/ast.hpp"

namespace arrow {
namespace ast {

Visitor::~Visitor() noexcept {
}

void Visitor::run(Node& x) {
  x.accept(*this);
}

}  // namespace ast
}  // namespace arrow
