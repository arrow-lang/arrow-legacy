// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_AST_VISITOR_H
#define ARROW_AST_VISITOR_H 1

#include "arrow/ast/nodes.hpp"
#include "arrow/log.hpp"

namespace arrow {
namespace ast {

struct Node;

struct Visitor {
  virtual ~Visitor() noexcept;

  void run(Node&);

  // Unreachable (unless some serious shit goes down)
  virtual void visit_node(Node&) { /* LCOV_EXCL_LINE */ }

  // Module (top-level)
  virtual void visit_module(Module&) { /* LCOV_EXCL_LINE */ }

  // Block (scope)
  virtual void visit_block(Block&);

  // Literal
  virtual void visit_id(Identifier&) { /* LCOV_EXCL_LINE */ }
  virtual void visit_none(None&) { /* LCOV_EXCL_LINE */ }
  virtual void visit_bool(Boolean&) { /* LCOV_EXCL_LINE */ }
  virtual void visit_str(String&) { /* LCOV_EXCL_LINE */ }
  virtual void visit_int(Integer&) { /* LCOV_EXCL_LINE */ }
  virtual void visit_float(Float&) { /* LCOV_EXCL_LINE */ }
  virtual void visit_tuple(Tuple&) { /* LCOV_EXCL_LINE */ }

  // Import
  virtual void visit_import(Import&) { /* LCOV_EXCL_LINE */ }

  // Select
  virtual void visit_select(Select&);
  virtual void visit_select_branch(SelectBranch&);

  // Loop
  virtual void visit_loop(Loop&);
  virtual void visit_break(Break&) { /* LCOV_EXCL_LINE */ }
  virtual void visit_continue(Continue&) { /* LCOV_EXCL_LINE */ }

  // Call
  virtual void visit_call(Call&);
  virtual void visit_argument(Argument&);

  // Structure
  virtual void visit_struct(Structure&) { /* LCOV_EXCL_LINE */ }
  virtual void visit_member(Member&) { /* LCOV_EXCL_LINE */ }

  // Function
  virtual void visit_function(Function&) { /* LCOV_EXCL_LINE */ }
  virtual void visit_extern_function(ExternFunction&) { /* LCOV_EXCL_LINE */ }
  virtual void visit_parameter(Parameter&) { /* LCOV_EXCL_LINE */ }
  virtual void visit_return(Return&);

  // Slot
  virtual void visit_slot(Slot&) { /* LCOV_EXCL_LINE */ }
  virtual void visit_extern_slot(ExternSlot&) { /* LCOV_EXCL_LINE */ }

  // Unary
  virtual void visit_identity(Identity&);
  virtual void visit_negate(Negate&);
  virtual void visit_bit_not(BitNot&);
  virtual void visit_deref(Dereference&);
  virtual void visit_not(Not&);
  virtual void visit_address_of(AddressOf&);

  // Binary
  virtual void visit_add(Add&);
  virtual void visit_sub(Sub&);
  virtual void visit_mul(Mul&);
  virtual void visit_div(Div&);
  virtual void visit_mod(Mod&);
  virtual void visit_eq(EqualTo&);
  virtual void visit_ne(NotEqualTo&);
  virtual void visit_lt(LessThan&);
  virtual void visit_le(LessThanOrEqualTo&);
  virtual void visit_ge(GreaterThanOrEqualTo&);
  virtual void visit_gt(GreaterThan&);
  virtual void visit_bit_and(BitAnd&);
  virtual void visit_bit_xor(BitXor&);
  virtual void visit_bit_or(BitOr&);
  virtual void visit_and(And&);
  virtual void visit_or(Or&);

  // Assignment
  virtual void visit_assign(Assign&);

  // Cast
  virtual void visit_cast(Cast&);

  // Path (member-of)
  virtual void visit_path(Path&);

  // Pattern
  virtual void visit_pattern_wildcard(PatternWildcard&) { /* LCOV_EXCL_LINE */ }
  virtual void visit_pattern_identifier(PatternIdentifier&) { /* LCOV_EXCL_LINE */ }
  virtual void visit_pattern_literal(PatternLiteral&) { /* LCOV_EXCL_LINE */ }
  virtual void visit_pattern_tuple(PatternTuple&) { /* LCOV_EXCL_LINE */ }

  // Type
  virtual void visit_type_none(TypeNone&) { /* LCOV_EXCL_LINE */ }
  virtual void visit_type_path(TypePath&) { /* LCOV_EXCL_LINE */ }
  virtual void visit_type_pointer(TypePointer&) { /* LCOV_EXCL_LINE */ }
  virtual void visit_type_tuple(TypeTuple&) { /* LCOV_EXCL_LINE */ }
  virtual void visit_type_parameter(TypeParameter&) { /* LCOV_EXCL_LINE */ }
  virtual void visit_type_function(TypeFunction&) { /* LCOV_EXCL_LINE */ }

 private:
  void do_unary(ast::Unary& x);
  void do_binary(ast::Binary& x);
};

}  // namespace ast
}  // namespace arrow

#endif  // ARROW_AST_VISITOR_H
