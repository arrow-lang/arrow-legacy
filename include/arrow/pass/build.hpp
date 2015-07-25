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

  // Module (top-level)
  virtual void visit_module(ast::Module&);

  // Block (scope)
  virtual void visit_block(ast::Block&);

  // Slot
  virtual void visit_slot(ast::Slot&);

  // Function
  virtual void visit_function(ast::Function&);
  virtual void visit_return(ast::Return&);

  // Literal
  virtual void visit_id(ast::Identifier&);
  virtual void visit_none(ast::None&);
  virtual void visit_bool(ast::Boolean&);
  // virtual void visit_str(ast::String&);
  virtual void visit_int(ast::Integer&);
  virtual void visit_float(ast::Float&);
  virtual void visit_tuple(ast::Tuple&);

  // Binary
  virtual void visit_assign(ast::Assign&);

  // Call
  virtual void visit_call(ast::Call& x);

 private:
  Ref<code::Value> do_cast(
    Ref<code::Value> value, ast::Node& node, Ref<code::Type> to_type,
    bool explicit_);

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
