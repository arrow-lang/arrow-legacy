// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_BUILDER_H
#define ARROW_BUILDER_H 1

#include <stack>
#include "arrow/ast.hpp"
#include "arrow/code.hpp"

namespace arrow {

class Generator;

class Builder : public ast::Visitor {
 public:
  explicit Builder(Generator& g, code::Scope& _scope);

  virtual ~Builder() noexcept;

  virtual void visit(ast::Node&) { }
  virtual void visit(ast::TextNode&) { }
  virtual void visit(ast::Identifier&) override;
  virtual void visit(ast::Module&) override;
  virtual void visit(ast::Function&) override;
  virtual void visit(ast::Call&) override;
  virtual void visit(ast::Break&) { }
  virtual void visit(ast::Return&) { }
  virtual void visit(ast::Integer&) { }
  virtual void visit(ast::Float&) { }
  virtual void visit(ast::Boolean&) { }
  virtual void visit(ast::Promote&) { }
  virtual void visit(ast::NegateLogical&) { }
  virtual void visit(ast::NegateBit&) { }
  virtual void visit(ast::NegateNumeric&) { }
  virtual void visit(ast::Assign&) { }
  virtual void visit(ast::AssignAdd&) { }
  virtual void visit(ast::AssignSub&) { }
  virtual void visit(ast::AssignMul&) { }
  virtual void visit(ast::AssignDiv&) { }
  virtual void visit(ast::AssignMod&) { }
  virtual void visit(ast::AssignBitAnd&) { }
  virtual void visit(ast::AssignBitXor&) { }
  virtual void visit(ast::AssignBitOr&) { }
  virtual void visit(ast::And&) { }
  virtual void visit(ast::Or&) { }
  virtual void visit(ast::EqualTo&) { }
  virtual void visit(ast::NotEqualTo&) { }
  virtual void visit(ast::LessThan&) { }
  virtual void visit(ast::LessThanOrEqualTo&) { }
  virtual void visit(ast::GreaterThanOrEqualTo&) { }
  virtual void visit(ast::GreaterThan&) { }
  virtual void visit(ast::BitAnd&) { }
  virtual void visit(ast::BitXor&) { }
  virtual void visit(ast::BitOr&) { }
  virtual void visit(ast::Add&) { }
  virtual void visit(ast::Sub&) { }
  virtual void visit(ast::Mul&) { }
  virtual void visit(ast::Div&) { }
  virtual void visit(ast::Mod&) { }

 private:
  Generator& _g;
  code::Scope& _scope;
  code::Scope* _cs;
  std::stack<std::shared_ptr<code::Item>> _stack;

  void build(
    ast::Node& node, code::Scope* scope = nullptr);

  std::shared_ptr<code::Item> build_scalar(
    ast::Node& node, code::Scope* scope = nullptr);

};

} // namespace arrow {

#endif // ARROW_BUILDER_H
