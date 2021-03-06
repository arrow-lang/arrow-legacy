// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_AST_SHOW_H
#define ARROW_AST_SHOW_H 1

#include <functional>

#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

#include "arrow/ast/visitor.hpp"
#include "arrow/log.hpp"

namespace arrow {
namespace ast {

struct Node;

struct Show : Visitor {
  Show();

  virtual ~Show() noexcept;

  virtual void run(Node&);

  virtual void visit_id(Identifier&);
  virtual void visit_module(Module&);
  virtual void visit_block(Block&);
  virtual void visit_import(Import&);

  virtual void visit_bool(Boolean&);
  virtual void visit_str(String&);
  virtual void visit_int(Integer&);
  virtual void visit_float(Float&);
  virtual void visit_none(None&);
  virtual void visit_array(Array&);

  virtual void visit_loop(Loop&);
  virtual void visit_break(Break&);
  virtual void visit_continue(Continue&);

  virtual void visit_identity(Identity&);
  virtual void visit_negate(Negate&);
  virtual void visit_bit_not(BitNot&);
  virtual void visit_deref(Dereference&);
  virtual void visit_not(Not&);
  virtual void visit_address_of(AddressOf&);

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

  virtual void visit_assign(Assign&);

  virtual void visit_and(And&);
  virtual void visit_or(Or&);

  virtual void visit_cast(Cast&);
  virtual void visit_index(Index&);
  virtual void visit_path(Path&);

  virtual void visit_call(Call&);
  virtual void visit_argument(Argument&);

  virtual void visit_struct(Structure&);
  virtual void visit_member(Member&);

  virtual void visit_slot(Slot&);
  virtual void visit_extern_slot(ExternSlot&);

  virtual void visit_pattern_wildcard(PatternWildcard&);
  virtual void visit_pattern_identifier(PatternIdentifier&);
  virtual void visit_pattern_literal(PatternLiteral&);
  virtual void visit_pattern_tuple(PatternTuple&);

  virtual void visit_type_none(TypeNone&);
  virtual void visit_type_path(TypePath&);
  virtual void visit_type_pointer(TypePointer&);
  virtual void visit_type_tuple(TypeTuple&);
  virtual void visit_type_parameter(TypeParameter&);
  virtual void visit_type_function(TypeFunction&);
  virtual void visit_type_array(TypeArray&);
  virtual void visit_type_of(TypeOf&);

  virtual void visit_tuple(Tuple&);

  virtual void visit_parameter(Parameter&);
  virtual void visit_function(Function&);
  virtual void visit_extern_function(ExternFunction&);
  virtual void visit_return(Return&);

  virtual void visit_conditional(Conditional&);
  virtual void visit_select(Select&);
  virtual void visit_select_branch(SelectBranch&);

  virtual void visit_alias(Alias&);

 private:
  void do_(const char* name, Node& x);
  void do_(const char* name, Node& x, std::function<void()> inner);

  void do_unary(const char* name, Unary& x);
  void do_binary(const char* name, Binary& x);

  rapidjson::StringBuffer _s;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> _w;

};

}  // namespace ast
}  // namespace arrow

#endif  // ARROW_AST_SHOW_H
