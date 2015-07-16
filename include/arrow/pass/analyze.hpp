// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_PASS_ANALYZE_H
#define ARROW_PASS_ANALYZE_H 1

#include "arrow/ast.hpp"
#include "arrow/compiler.hpp"
#include "arrow/code.hpp"

namespace arrow {
namespace pass {

class Analyze : public ast::Visitor {
 public:
  explicit Analyze(Ref<code::Scope> scope) : _scope(scope), _incomplete(false) {
  }

  virtual ~Analyze() noexcept;

  void run(ast::Node& x);

  // Blocks (or nodes containing blocks)
  virtual void visit_block(ast::Block& x);
  virtual void visit_loop(ast::Loop& x);
  virtual void visit_select(ast::Select& x);
  virtual void visit_select_branch(ast::SelectBranch& x);

  // Unary
  virtual void visit_identity(ast::Identity& x);
  virtual void visit_negate(ast::Negate& x);
  virtual void visit_bit_not(ast::BitNot& x);
  virtual void visit_deref(ast::Dereference& x);
  virtual void visit_not(ast::Not& x);
  virtual void visit_address_of(ast::AddressOf& x);

  // Binary
  virtual void visit_add(ast::Add& x);
  virtual void visit_sub(ast::Sub& x);
  virtual void visit_mul(ast::Mul& x);
  virtual void visit_div(ast::Div& x);
  virtual void visit_mod(ast::Mod& x);
  virtual void visit_eq(ast::EqualTo& x);
  virtual void visit_ne(ast::NotEqualTo& x);
  virtual void visit_lt(ast::LessThan& x);
  virtual void visit_le(ast::LessThanOrEqualTo& x);
  virtual void visit_ge(ast::GreaterThanOrEqualTo& x);
  virtual void visit_gt(ast::GreaterThan& x);
  virtual void visit_bit_and(ast::BitAnd& x);
  virtual void visit_bit_xor(ast::BitXor& x);
  virtual void visit_bit_or(ast::BitOr& x);
  virtual void visit_and(ast::And& x);
  virtual void visit_or(ast::Or& x);

  // TODO(mehcode): Misc.
  virtual void visit_slot(ast::Slot& x);
  // virtual void visit_call(ast::Call& x);
  // virtual void visit_argument(ast::Argument& x);
  // virtual void visit_tuple(ast::Tuple&);
  // virtual void visit_path(ast::Path& x);
  // virtual void visit_cast(ast::Cast& x);

  // Use [!]
  virtual void visit_id(ast::Identifier& x);

  // Assign [!]
  virtual void visit_assign(ast::Assign& x);

 private:
  void do_unary(ast::Unary& x);
  void do_binary(ast::Binary& x);

  bool _expand_pattern(
    ast::Pattern& pattern,
    Ref<code::Type> type_annotation,
    Ref<code::Type> type_initializer);

  bool _expand_assign(ast::Assign& node, ast::Node& lhs, Ref<code::Type> type);

  // The scope to emplace the exposed items into.
  Ref<code::Scope> _scope;

  // Data flow analysis
  struct Assignment {
    /// A definite assignment is one that will /always/ happen.
    bool is_definite;

    /// Usage count of the value from this assignment.
    unsigned uses;

    /// Type of the assignment.
    Ref<code::Type> type;
  };

  struct Declaration {
    /// Type of the declaration (can be null if there was no annotation).
    Ref<code::Type> type;

    /// Name of the declaration.
    std::string name;

    /// If the declaration refers to a mutable slot.
    bool is_mutable;
  };

  bool _incomplete;
  std::unordered_map<std::string, ast::Node*> _x_name;
  std::unordered_map<ast::Node*, Declaration> _x_declare;
  std::unordered_map<ast::Node*, std::vector<Assignment>> _x_assign;
};

}  // namespace pass
}  // namespace arrow

#endif  // ARROW_PASS_ANALYZE_H
