// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_AST_VISITOR_H
#define ARROW_AST_VISITOR_H 1

#include "arrow/ast/nodes.hpp"

namespace arrow {
namespace ast {

struct Node;

class AbstractVisitor {
 public:
  virtual ~AbstractVisitor() noexcept;

  virtual void run(Node&);

 private:
  friend struct Node;
  friend struct TextNode;
  friend struct Unary;
  friend struct Binary;
  friend struct Identifier;
  friend struct Module;
  friend struct Function;
  friend struct Parameter;
  friend struct Call;
  friend struct Slot;
  friend struct Break;
  friend struct Return;
  friend struct Integer;
  friend struct Float;
  friend struct Boolean;
  friend struct String;
  friend struct Promote;
  friend struct NegateLogical;
  friend struct NegateBit;
  friend struct NegateNumeric;
  friend struct Assign;
  friend struct AssignAdd;
  friend struct AssignSub;
  friend struct AssignMul;
  friend struct AssignDiv;
  friend struct AssignMod;
  friend struct AssignBitAnd;
  friend struct AssignBitXor;
  friend struct AssignBitOr;
  friend struct And;
  friend struct Or;
  friend struct EqualTo;
  friend struct NotEqualTo;
  friend struct LessThan;
  friend struct LessThanOrEqualTo;
  friend struct GreaterThanOrEqualTo;
  friend struct GreaterThan;
  friend struct BitAnd;
  friend struct BitXor;
  friend struct BitOr;
  friend struct Add;
  friend struct Sub;
  friend struct Mul;
  friend struct Div;
  friend struct Mod;

  virtual void visit(Node&) { }
  virtual void visit(TextNode&) { }
  virtual void visit(Identifier&) { }
  virtual void visit(Module&) { }
  virtual void visit(Function&) { }
  virtual void visit(Parameter&) { }
  virtual void visit(Call&) { }
  virtual void visit(Slot&) { }
  virtual void visit(Break&) { }
  virtual void visit(Return&) { }
  virtual void visit(Integer&) { }
  virtual void visit(Float&) { }
  virtual void visit(Boolean&) { }
  virtual void visit(String&) { }
  virtual void visit(Promote&) { }
  virtual void visit(NegateLogical&) { }
  virtual void visit(NegateBit&) { }
  virtual void visit(NegateNumeric&) { }
  virtual void visit(Assign&) { }
  virtual void visit(AssignAdd&) { }
  virtual void visit(AssignSub&) { }
  virtual void visit(AssignMul&) { }
  virtual void visit(AssignDiv&) { }
  virtual void visit(AssignMod&) { }
  virtual void visit(AssignBitAnd&) { }
  virtual void visit(AssignBitXor&) { }
  virtual void visit(AssignBitOr&) { }
  virtual void visit(And&) { }
  virtual void visit(Or&) { }
  virtual void visit(EqualTo&) { }
  virtual void visit(NotEqualTo&) { }
  virtual void visit(LessThan&) { }
  virtual void visit(LessThanOrEqualTo&) { }
  virtual void visit(GreaterThanOrEqualTo&) { }
  virtual void visit(GreaterThan&) { }
  virtual void visit(BitAnd&) { }
  virtual void visit(BitXor&) { }
  virtual void visit(BitOr&) { }
  virtual void visit(Add&) { }
  virtual void visit(Sub&) { }
  virtual void visit(Mul&) { }
  virtual void visit(Div&) { }
  virtual void visit(Mod&) { }
};

class Visitor : public AbstractVisitor {
 public:
  virtual ~Visitor() noexcept;

  virtual void visit(Node&) { }
  virtual void visit(TextNode&) { }

  virtual void visit(Identifier&);
  virtual void visit(Module&);
  virtual void visit(Function&);
  virtual void visit(Parameter&);
  virtual void visit(Call&);
  virtual void visit(Slot&);
  virtual void visit(Break&);
  virtual void visit(Return&);
  virtual void visit(Integer&);
  virtual void visit(Float&);
  virtual void visit(Boolean&);
  virtual void visit(String&);
  virtual void visit(Promote&);
  virtual void visit(NegateLogical&);
  virtual void visit(NegateBit&);
  virtual void visit(NegateNumeric&);
  virtual void visit(Assign&);
  virtual void visit(AssignAdd&);
  virtual void visit(AssignSub&);
  virtual void visit(AssignMul&);
  virtual void visit(AssignDiv&);
  virtual void visit(AssignMod&);
  virtual void visit(AssignBitAnd&);
  virtual void visit(AssignBitXor&);
  virtual void visit(AssignBitOr&);
  virtual void visit(And&);
  virtual void visit(Or&);
  virtual void visit(EqualTo&);
  virtual void visit(NotEqualTo&);
  virtual void visit(LessThan&);
  virtual void visit(LessThanOrEqualTo&);
  virtual void visit(GreaterThanOrEqualTo&);
  virtual void visit(GreaterThan&);
  virtual void visit(BitAnd&);
  virtual void visit(BitXor&);
  virtual void visit(BitOr&);
  virtual void visit(Add&);
  virtual void visit(Sub&);
  virtual void visit(Mul&);
  virtual void visit(Div&);
  virtual void visit(Mod&);

  virtual void visit_id(Identifier&) { }
  virtual void visit_module(Module&);
  virtual void visit_function(Function&) { }
  virtual void visit_parameter(Parameter&) { }
  virtual void visit_call(Call&) { }
  virtual void visit_slot(Slot&) { }
  virtual void visit_break(Break&) { }
  virtual void visit_return(Return&) { }
  virtual void visit_int(Integer&) { }
  virtual void visit_float(Float&) { }
  virtual void visit_bool(Boolean&) { }
  virtual void visit_str(String&) { }
  virtual void visit_promote(Promote&) { }
  virtual void visit_negate_logical(NegateLogical&) { }
  virtual void visit_negate_bit(NegateBit&) { }
  virtual void visit_negate_numeric(NegateNumeric&) { }
  virtual void visit_assign(Assign&) { }
  virtual void visit_assign_add(AssignAdd&) { }
  virtual void visit_assign_sub(AssignSub&) { }
  virtual void visit_assign_mul(AssignMul&) { }
  virtual void visit_assign_div(AssignDiv&) { }
  virtual void visit_assign_mod(AssignMod&) { }
  virtual void visit_assign_bit_and(AssignBitAnd&) { }
  virtual void visit_assign_bit_xor(AssignBitXor&) { }
  virtual void visit_assign_bit_or(AssignBitOr&) { }
  virtual void visit_and(And&) { }
  virtual void visit_or(Or&) { }
  virtual void visit_eq(EqualTo&) { }
  virtual void visit_ne(NotEqualTo&) { }
  virtual void visit_lt(LessThan&) { }
  virtual void visit_le(LessThanOrEqualTo&) { }
  virtual void visit_ge(GreaterThanOrEqualTo&) { }
  virtual void visit_gt(GreaterThan&) { }
  virtual void visit_bit_and(BitAnd&) { }
  virtual void visit_bit_xor(BitXor&) { }
  virtual void visit_bit_or(BitOr&) { }
  virtual void visit_add(Add&) { }
  virtual void visit_sub(Sub&) { }
  virtual void visit_mul(Mul&) { }
  virtual void visit_div(Div&) { }
  virtual void visit_mod(Mod&) { }
};

}  // namespace ast
}  // namespace arrow

#endif  // ARROW_AST_VISITOR_H
