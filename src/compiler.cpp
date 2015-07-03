// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <boost/filesystem.hpp>

#include "arrow/compiler.hpp"

namespace fs = boost::filesystem;

namespace arrow {

Compiler::Compiler() {
}

Compiler::~Compiler() noexcept {
}

void Compiler::compile(Ref<ast::Node> node) {
  // Multi-pass
  //  - Fold (compact AST nodes where possible)
  //  - Type (discern the type of a type expression)
  //  - Resolve (discern the type of a value expression)
  //  - Build (render the LLVM IR)
  //  - Expose (hoist names)
}

void Compiler::print() {
  std::printf("compiled..\n");
}

}  // namespace arrow
