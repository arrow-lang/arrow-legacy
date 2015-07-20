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
  explicit AnalyzeType(Ref<code::Scope> scope)
    : _scope(scope), _incomplete(false) {
  }

  virtual ~AnalyzeType() noexcept;

  void run(ast::Node& x);

  // Slot
  virtual void visit_slot(ast::Slot& x);
  virtual void visit_extern_slot(ast::ExternSlot& x);

  // Function
  virtual void visit_extern_function(ast::ExternFunction& x);

  // Use [!]
  virtual void visit_id(ast::Identifier& x);

  // Assign [!]
  virtual void visit_assign(ast::Assign& x);

 private:
  bool _expand_pattern(
    ast::Pattern& pattern,
    Ref<code::Type> type_annotation,
    Ref<code::Type> type_initializer);

  bool _expand_assign(
    ast::Assign& node, ast::Node& lhs, Ref<code::Type> type);

  Ref<code::Scope> _scope;

  // // Data flow analysis
  // struct Assignment {
  //   /// A definite assignment is one that will /always/ happen.
  //   bool is_definite;
  //
  //   /// Usage count of the value from this assignment.
  //   unsigned uses;
  //
  //   /// Type of the assignment.
  //   Ref<code::Type> type;
  // };
  //
  // struct Declaration {
  //   /// Type of the declaration (can be null if there was no annotation).
  //   Ref<code::Type> type;
  //
  //   /// Name of the declaration.
  //   std::string name;
  //
  //   /// If the declaration refers to a mutable slot.
  //   bool is_mutable;
  //
  //   /// If the declaration is external.
  //   bool is_external;
  // };

  bool _incomplete;
  // std::unordered_map<std::string, ast::Node*> _x_name;
  // std::unordered_map<ast::Node*, Declaration> _x_declare;
  // std::unordered_map<ast::Node*, std::vector<Assignment>> _x_assign;
};

}  // namespace pass
}  // namespace arrow

#endif  // ARROW_PASS_ANALYZE_TYPE_H
