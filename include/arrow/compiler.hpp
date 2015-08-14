// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_COMPILER_H
#define ARROW_COMPILER_H 1

#include <cstdarg>
#include <deque>
#include <vector>
#include <stack>

#include "arrow/ref.hpp"
#include "arrow/parser.hpp"
#include "arrow/ast.hpp"
#include "arrow/llvm.hpp"
#include "arrow/code/scope.hpp"
#include "arrow/code/module.hpp"

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

    /// Map of native modules that are / have been compiled
    std::unordered_map<ast::Node*, Ref<code::Module>> modules_by_context;
    std::unordered_map<std::string, Ref<code::Module>> modules_by_pathname;
    std::deque<Ref<code::Module>> modules;

    /// Stack of loops
    struct LoopFrame {
      LLVMBasicBlockRef condition;
      LLVMBasicBlockRef merge;
    };

    std::stack<LoopFrame> loops;
  };

  Compiler(bool verify);

  ~Compiler() noexcept;

  void initialize();

  void compile(const std::string& name, Ref<ast::Node> node);

  void print();
  int run(int argc, char** argv, char** environ);

 private:
  Context _ctx;

  /// Top-level scope (contains built-ins)
  Ref<code::Scope> _scope;

  bool _verify;
};

}  // namespace arrow

#endif  // ARROW_COMPILER_H
