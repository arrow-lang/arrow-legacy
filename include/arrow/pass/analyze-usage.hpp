// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_PASS_ANALYZE_USAGE_H
#define ARROW_PASS_ANALYZE_USAGE_H 1

#include "arrow/ast.hpp"
#include "arrow/compiler.hpp"
#include "arrow/code.hpp"

namespace arrow {
namespace pass {

class AnalyzeUsage : public ast::Visitor {
 public:
  explicit AnalyzeUsage(Compiler::Context& ctx, Ref<code::Scope> scope)
    : _ctx(ctx), _scope(scope) {
  }

  virtual ~AnalyzeUsage() noexcept;

  // Block (scope)
  virtual void visit_block(ast::Block& x);

  // Module
  virtual void visit_module(ast::Module& x);

  // Select
  virtual void visit_select(ast::Select& x);

  // Slot
  virtual void visit_slot(ast::Slot& x);

  // Function
  virtual void visit_function(ast::Function& x);

  // Call
  virtual void visit_call(ast::Call& x);
  virtual void visit_argument(ast::Argument& x);

  // Use [!]
  virtual void visit_id(ast::Identifier& x);
  virtual void visit_path(ast::Path& x);

  // Assign [!]
  virtual void visit_assign(ast::Assign& x);

 private:
  bool _expand_pattern(ast::Pattern& pattern, bool has_initializer);
  bool _expand_assign(ast::Node& lhs, ast::Assign* context);

  void _enter_block(arrow::ast::Node& x);
  void _exit_block(bool is_definite = true);

  void do_path(ast::Path& x, ast::Node* in_assign = nullptr);

  void do_realize_function(
    ast::Node& context, code::TypeFunction& fn,
    bool is_definite = false
  );

  void do_use(ast::Node& context, code::Slot& item);
  void do_assign(
    ast::Node& context, code::Slot* item, bool is_definite);

  // The active compiler context.
  Compiler::Context& _ctx;

  Ref<code::Scope> _scope;

  std::unordered_map<Ref<code::Block>, std::deque<code::Slot*>> _assign;
  std::unordered_set<code::Slot*> _use;
};

}  // namespace pass
}  // namespace arrow

#endif  // ARROW_PASS_ANALYZE_USAGE_H
