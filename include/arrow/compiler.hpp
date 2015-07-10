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
    /// LLVM module that contains the LLVM IR
    LLVMModuleRef mod;

    /// LLVM instruction builder that greatly simplifies
    /// IR generation with LLVM
    LLVMBuilderRef irb;

    /// LLVM target machine
    LLVMTargetMachineRef target_machine;

    /// LLVM target data (layout)
    LLVMTargetDataRef data_layout;
  };

  Compiler();

  ~Compiler() noexcept;

  void initialize();

  void compile(const std::string& name, Ref<ast::Node> node);

  void print();

 private:
  Context _ctx;

  /// Top-level scope (contains built-ins)
  Ref<code::Scope> _scope;
};

}  // namespace arrow

#endif  // ARROW_COMPILER_H
