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

class Show : public AbstractVisitor {
 public:
  explicit Show(std::ostream& os);

  virtual ~Show() noexcept;

  virtual void run(Node&);

 private:
  void handle_unary(const std::string& name, Unary&);
  void handle_binary(const std::string& name, Binary&);

  virtual void visit(Node&) { }
  virtual void visit(TextNode&) { }
  virtual void visit(Identifier&);
  virtual void visit(Module&);
  virtual void visit(AbstractFunction&) { }
  virtual void visit(Function&);
  virtual void visit(ExternalFunction&);
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
  virtual void visit(Select&);
  virtual void visit(SelectBranch&);

  boost::property_tree::ptree& _el();

  std::ostream& _stream;

  boost::property_tree::ptree _tree;
  std::stack<boost::property_tree::ptree*> _ctx;
};

}  // namespace ast
}  // namespace arrow

#endif  // ARROW_AST_SHOW_H
