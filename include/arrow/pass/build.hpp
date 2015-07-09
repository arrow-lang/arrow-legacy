// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_PASS_MODULE_H
#define ARROW_PASS_MODULE_H 1

#include "arrow/ast.hpp"
#include "arrow/compiler.hpp"
#include "arrow/code.hpp"

namespace arrow {
namespace pass {

class Build : public ast::Visitor {
 public:
  Build(const Compiler::Context& ctx, code::Scope& scope)
      : _ctx(ctx), _scope(scope) {
  }

  virtual ~Build() noexcept;

  virtual void visit_module(ast::Module
    &);

 private:
  // The active compiler context.
  const Compiler::Context& _ctx;

  // The scope to emplace the exposed items into.
  code::Scope& _scope;

};

}  // namespace pass
}  // namespace arrow

#endif  // ARROW_PASS_MODULE_H
