// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_PASS_ANALYZE_MODULE_H
#define ARROW_PASS_ANALYZE_MODULE_H 1

#include "arrow/ast.hpp"
#include "arrow/compiler.hpp"
#include "arrow/code.hpp"

namespace arrow {
namespace pass {

class AnalyzeModule : public ast::Visitor {
 public:
  explicit AnalyzeModule(Compiler::Context& ctx, Ref<code::Scope> scope)
    : _ctx(ctx), _scope(scope) {
  }

  virtual ~AnalyzeModule() noexcept;

  void visit_module(ast::Module&);
  void visit_block(ast::Block&);
  void visit_id(ast::Identifier&);
  void visit_path(ast::Path&);
  void visit_function(ast::Function&);
  void visit_call(ast::Call&);

 private:
  Compiler::Context& _ctx;
  Ref<code::Scope> _scope;
};

}  // namespace pass
}  // namespace arrow

#endif  // ARROW_PASS_ANALYZE_MODULE_H
