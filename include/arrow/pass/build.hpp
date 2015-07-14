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
  Build(Compiler::Context& ctx, Ref<code::Scope> scope)
    : _ctx(ctx), _scope(scope) {
  }

  virtual ~Build() noexcept;

  Ref<code::Value> run_scalar(ast::Node& x) {
    ast::Visitor::run(x);

    return _stack.size() == 1 ? _stack.front() : nullptr;
  }

  virtual void visit_module(ast::Module&);
  virtual void visit_block(ast::Block&);
  virtual void visit_slot(ast::Slot&);

  // Literal
  virtual void visit_id(ast::Identifier&);
  // virtual void visit_none(ast::None&);
  virtual void visit_bool(ast::Boolean&);
  // virtual void visit_str(ast::String&);
  virtual void visit_int(ast::Integer&);
  virtual void visit_float(ast::Float&);
  virtual void visit_tuple(ast::Tuple&);

 private:
  /// The active compiler context.
  Compiler::Context& _ctx;

  /// The scope to emplace the exposed items into.
  Ref<code::Scope> _scope;

  /// Value stack for the generation process.
  std::deque<Ref<code::Value>> _stack;
};

}  // namespace pass
}  // namespace arrow

#endif  // ARROW_PASS_MODULE_H
