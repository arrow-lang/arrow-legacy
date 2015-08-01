// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_PASS_RESOLVE_H
#define ARROW_PASS_RESOLVE_H 1

#include "arrow/ast.hpp"
#include "arrow/code.hpp"

namespace arrow {
namespace pass {

class Resolve : public ast::Visitor {
 public:
  explicit Resolve(Ref<code::Scope> scope)
      : _scope(scope) {
  }

  virtual ~Resolve() noexcept;

  Ref<code::Type> run(ast::Node& x) {
    ast::Visitor::run(x);

    return _stack.size() > 0 ? _stack.front() : nullptr;
  }

  // Block (scope)
  virtual void visit_block(ast::Block&);

  // Literal
  virtual void visit_id(ast::Identifier&);
  virtual void visit_none(ast::None&);
  virtual void visit_bool(ast::Boolean&);
  virtual void visit_str(ast::String&);
  virtual void visit_int(ast::Integer&);
  virtual void visit_float(ast::Float&);
  virtual void visit_tuple(ast::Tuple&);

  // Function
  virtual void visit_function(ast::Function&);
  virtual void visit_extern_function(ast::ExternFunction&);
  virtual void visit_parameter(ast::Parameter&);

  // Call
  virtual void visit_call(ast::Call&);

  // Select
  virtual void visit_select(ast::Select&);
  virtual void visit_select_branch(ast::SelectBranch&);

  // Misc.
  virtual void visit_slot(ast::Slot&);
  virtual void visit_path(ast::Path&);

 private:
  Ref<code::Type> type_of(Ref<code::Item> item);

  // The scope to emplace the exposed items into.
  Ref<code::Scope> _scope;

  // Type stack for use during recursive process.
  std::deque<Ref<code::Type>> _stack;
};

}  // namespace pass
}  // namespace arrow

#endif  // ARROW_PASS_RESOLVE_H
