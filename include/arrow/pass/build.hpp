// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_PASS_BUILD_H
#define ARROW_PASS_BUILD_H 1

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
  virtual void visit_str(ast::String&);
  virtual void visit_int(ast::Integer&);
  virtual void visit_float(ast::Float&);
  virtual void visit_tuple(ast::Tuple&);

  // Unary
  // virtual void visit_identity(ast::Identity&);
  virtual void visit_negate(ast::Negate&);
  virtual void visit_bit_not(ast::BitNot&);
  virtual void visit_not(ast::Not&);
  // virtual void visit_deref(ast::Dereference&);
  // virtual void visit_address_of(ast::AddressOf&);

  // Binary
  virtual void visit_assign(ast::Assign&);
  virtual void visit_path(ast::Path&);

  // Call
  virtual void visit_call(ast::Call&);

  // Select
  virtual void visit_select(ast::Select&);
  // virtual void visit_select_branch(ast::SelectBranch&);

 private:
  void do_unary(
    ast::Unary& x, std::function<LLVMValueRef(Ref<code::Value>)> c);

  bool _expand_parameter_pattern(
    ast::Pattern& pattern, Ref<code::Value> value, Ref<code::Scope> scope);

  /// The active compiler context.
  Compiler::Context& _ctx;

  /// The scope to emplace the exposed items into.
  Ref<code::Scope> _scope;

  /// Value stack for the generation process.
  std::deque<Ref<code::Value>> _stack;
};

}  // namespace pass
}  // namespace arrow

#endif  // ARROW_PASS_BUILD_H
