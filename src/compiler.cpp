// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <boost/filesystem.hpp>

#include "arrow/compiler.hpp"
#include "arrow/pass/build.hpp"

namespace fs = boost::filesystem;

namespace arrow {

Compiler::Compiler() : _scope(new code::Scope()) {
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

  // Invoke the build pass on the given node (module)
  pass::Build(_ctx, _scope).run(*node);
}

void Compiler::print() {
  std::printf("compiled..\n");
}

}  // namespace arrow
