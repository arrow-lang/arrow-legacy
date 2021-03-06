// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_PASS_EXPOSE_H
#define ARROW_PASS_EXPOSE_H 1

#include "arrow/ast.hpp"
#include "arrow/compiler.hpp"
#include "arrow/code.hpp"

namespace arrow {
namespace pass {

class Expose : public ast::Visitor {
 public:
  Expose(Compiler::Context& ctx, Ref<code::Scope> scope)
      : _ctx(ctx), _scope(scope) {
  }

  virtual ~Expose() noexcept;

  virtual void visit_block(ast::Block&);

  virtual void visit_module(ast::Module&);
  virtual void visit_import(ast::Import&);
  virtual void visit_extern_slot(ast::ExternSlot&);
  virtual void visit_extern_function(ast::ExternFunction&);
  virtual void visit_function(ast::Function&);
  virtual void visit_slot(ast::Slot&);
  virtual void visit_struct(ast::Structure&);

 private:
  bool _expand_parameter_pattern(
    ast::Pattern& pattern, Ref<code::Scope> scope);

  // The active compiler context.
  Compiler::Context& _ctx;

  // The scope to emplace the exposed items into.
  Ref<code::Scope> _scope;
};

}  // namespace pass
}  // namespace arrow

#endif  // ARROW_PASS_EXPOSE_H
