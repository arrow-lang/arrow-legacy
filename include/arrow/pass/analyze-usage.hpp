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
  explicit AnalyzeUsage(Ref<code::Scope> scope)
    : _scope(scope) {
  }

  virtual ~AnalyzeUsage() noexcept;

  // void run(ast::Node& x);

  // Block (scope)
  virtual void visit_block(ast::Block& x);

  // Slot
  virtual void visit_slot(ast::Slot& x);
  // virtual void visit_extern_slot(ast::ExternSlot& x);

  // Function
  // virtual void visit_extern_function(ast::ExternFunction& x);

  // Use [!]
  virtual void visit_id(ast::Identifier& x);

  // Assign [!]
  virtual void visit_assign(ast::Assign& x);

 private:
  bool _expand_pattern(ast::Pattern& pattern, bool has_initializer);

  bool _expand_assign(ast::Node& lhs, ast::Assign* context);

  Ref<code::Scope> _scope;

  struct Assignment {
    explicit Assignment(bool is_definite)
      : is_definite(is_definite) {
    }

    /// Whether this is a definite assignment (or
    /// a possible one from a branch)
    bool is_definite;
  };

  std::unordered_map<ast::Node*, std::deque<Assignment>> _assign;
};

}  // namespace pass
}  // namespace arrow

#endif  // ARROW_PASS_ANALYZE_USAGE_H
