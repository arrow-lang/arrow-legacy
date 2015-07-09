// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_COMPILER_H
#define ARROW_COMPILER_H 1

#include <cstdarg>
#include <deque>
#include <vector>

#include "arrow/ref.hpp"
#include "arrow/parser.hpp"
#include "arrow/ast.hpp"
#include "arrow/llvm.hpp"
#include "arrow/code.hpp"

namespace arrow {

class Compiler {
 public:
  struct Context {
  };

  Compiler();

  ~Compiler() noexcept;

  void compile(Ref<ast::Node> node);

  void print();

 private:
  Context _ctx;

  /// Top-level scope (contains built-ins)
  code::Scope _scope;

};

}  // namespace arrow

#endif  // ARROW_COMPILER_H
