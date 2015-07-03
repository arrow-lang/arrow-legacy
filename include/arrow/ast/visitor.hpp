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

  virtual void run(Node&);

  virtual void visit_node(Node&) { }

  virtual void visit_id(Identifier&) { }
  virtual void visit_module(Module&) { }
  virtual void visit_block(Block&) { }
  virtual void visit_import(Import&) { }

  virtual void visit_none(None&) { }
  virtual void visit_bool(Boolean&) { }
  virtual void visit_str(String&) { }
  virtual void visit_int(Integer&) { }
  virtual void visit_float(Float&) { }

  virtual void visit_loop(Loop&) { }
  virtual void visit_break(Break&) { }
  virtual void visit_continue(Continue&) { }

  virtual void visit_identity(Identity&) { }
  virtual void visit_negate(Negate&) { }
  virtual void visit_bit_not(BitNot&) { }
  virtual void visit_deref(Dereference&) { }
  virtual void visit_not(Not&) { }
  virtual void visit_address_of(AddressOf&) { }

  virtual void visit_add(Add&) { }
  virtual void visit_sub(Sub&) { }
  virtual void visit_mul(Mul&) { }
  virtual void visit_div(Div&) { }
  virtual void visit_mod(Mod&) { }

  virtual void visit_eq(EqualTo&) { }
  virtual void visit_ne(NotEqualTo&) { }
  virtual void visit_lt(LessThan&) { }
  virtual void visit_le(LessThanOrEqualTo&) { }
  virtual void visit_ge(GreaterThanOrEqualTo&) { }
  virtual void visit_gt(GreaterThan&) { }

  virtual void visit_bit_and(BitAnd&) { }
  virtual void visit_bit_xor(BitXor&) { }
  virtual void visit_bit_or(BitOr&) { }

  virtual void visit_assign(Assign&) { }

  virtual void visit_and(And&) { }
  virtual void visit_or(Or&) { }

  virtual void visit_cast(Cast&) { }
  virtual void visit_path(Path&) { }

  virtual void visit_call(Call&) { }
  virtual void visit_argument(Argument&) { }

  virtual void visit_parameter(Parameter&) { }
  virtual void visit_function(Function&) { }
  virtual void visit_extern_function(ExternFunction&) { }
  virtual void visit_return(Return&) { }

  virtual void visit_slot(Slot&) { }
  virtual void visit_extern_slot(ExternSlot&) { }

  virtual void visit_pattern_wildcard(PatternWildcard&) { }
  virtual void visit_pattern_identifier(PatternIdentifier&) { }
  virtual void visit_pattern_literal(PatternLiteral&) { }
  virtual void visit_pattern_tuple(PatternTuple&) { }

  virtual void visit_type_none(TypeNone&) { }
  virtual void visit_type_path(TypePath&) { }
  virtual void visit_type_pointer(TypePointer&) { }
  virtual void visit_type_tuple(TypeTuple&) { }

  virtual void visit_tuple(Tuple&) { }

};

}  // namespace ast
}  // namespace arrow

#endif  // ARROW_AST_VISITOR_H
