// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_AST_SHOW_H
#define ARROW_AST_SHOW_H 1

#include <boost/property_tree/ptree.hpp>

#include <string>
#include <stack>

#include "arrow/ast/nodes.hpp"
#include "arrow/ast/visitor.hpp"

namespace arrow {
namespace ast {

class Show : public Visitor {
 public:
  explicit Show(std::ostream& os);

  virtual ~Show() noexcept;

  virtual void run(Node&);

  virtual void visit_id(Identifier&);
  virtual void visit_module(Module&);
  virtual void visit_extern_function(ExternalFunction&);
  virtual void visit_function(Function&);
  virtual void visit_parameter(Parameter&);
  virtual void visit_call(Call&);
  virtual void visit_slot(Slot&);
  virtual void visit_break(Break&);
  virtual void visit_return(Return&);
  virtual void visit_int(Integer&);
  virtual void visit_float(Float&);
  virtual void visit_bool(Boolean&);
  virtual void visit_str(String&);
  virtual void visit_promote(Promote&);
  virtual void visit_negate_logical(NegateLogical&);
  virtual void visit_negate_bit(NegateBit&);
  virtual void visit_negate_numeric(NegateNumeric&);
  virtual void visit_assign(Assign&);
  virtual void visit_assign_add(AssignAdd&);
  virtual void visit_assign_sub(AssignSub&);
  virtual void visit_assign_mul(AssignMul&);
  virtual void visit_assign_div(AssignDiv&);
  virtual void visit_assign_mod(AssignMod&);
  virtual void visit_assign_bit_and(AssignBitAnd&);
  virtual void visit_assign_bit_xor(AssignBitXor&);
  virtual void visit_assign_bit_or(AssignBitOr&);
  virtual void visit_and(And&);
  virtual void visit_or(Or&);
  virtual void visit_eq(EqualTo&);
  virtual void visit_ne(NotEqualTo&);
  virtual void visit_lt(LessThan&);
  virtual void visit_le(LessThanOrEqualTo&);
  virtual void visit_ge(GreaterThanOrEqualTo&);
  virtual void visit_gt(GreaterThan&);
  virtual void visit_bit_and(BitAnd&);
  virtual void visit_bit_xor(BitXor&);
  virtual void visit_bit_or(BitOr&);
  virtual void visit_add(Add&);
  virtual void visit_sub(Sub&);
  virtual void visit_mul(Mul&);
  virtual void visit_div(Div&);
  virtual void visit_mod(Mod&);
  virtual void visit_select(Select&);
  virtual void visit_select_branch(SelectBranch&);
  virtual void visit_loop(Loop&);

 private:
  void handle_unary(const std::string& name, Unary&);
  void handle_binary(const std::string& name, Binary&);

  boost::property_tree::ptree& _el();

  std::ostream& _stream;

  boost::property_tree::ptree _tree;
  std::stack<boost::property_tree::ptree*> _ctx;
};

}  // namespace ast
}  // namespace arrow

#endif  // ARROW_AST_SHOW_H
