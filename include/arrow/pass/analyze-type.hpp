// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_PASS_ANALYZE_TYPE_H
#define ARROW_PASS_ANALYZE_TYPE_H 1

#include "arrow/ast.hpp"
#include "arrow/compiler.hpp"
#include "arrow/code.hpp"

namespace arrow {
namespace pass {

class AnalyzeType : public ast::Visitor {
 public:
  explicit AnalyzeType(Compiler::Context& ctx, Ref<code::Scope> scope)
    : _ctx(ctx), _scope(scope), _incomplete(false) {
  }

  virtual ~AnalyzeType() noexcept;

  void run(ast::Node& x);

  // Block
  virtual void visit_block(ast::Block& x);

  // Module
  virtual void visit_module(ast::Module&);
  virtual void visit_import(ast::Import&);

  // Slot
  virtual void visit_slot(ast::Slot& x);

  // Function
  virtual void visit_function(ast::Function& x);

  // Structure
  virtual void visit_struct(ast::Structure& x);

  // Use [!]
  // virtual void visit_id(ast::Identifier& x);

  // Assign [!]
  virtual void visit_assign(ast::Assign& x);

 private:
  bool _expand_pattern(
    ast::Pattern& pattern,
    Ref<code::Type> type_annotation,
    Ref<code::Type> type_initializer);

  bool _expand_parameter_pattern(
    ast::Pattern& pattern, Ref<code::Type> type, Ref<code::Scope> scope);

  bool _expand_assign(
    ast::Assign& node, ast::Node& lhs, Ref<code::Type> type);

  // The active compiler context.
  Compiler::Context& _ctx;

  Ref<code::Scope> _scope;

  struct Assignment {
    /// Type of the assignment.
    Ref<code::Type> type;
  };

  struct Use {
    /// Type of the use.
    Ref<code::Type> type;
  };

  bool _incomplete;
  std::unordered_map<code::Slot*, std::vector<Assignment>> _assign;
  std::unordered_map<code::Slot*, std::vector<Use>> _use;
  std::unordered_set<code::Module*> _modules;
};

}  // namespace pass
}  // namespace arrow

#endif  // ARROW_PASS_ANALYZE_TYPE_H
