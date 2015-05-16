// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_RESOLVE_H
#define ARROW_RESOLVE_H 1

#include <stack>

#include "arrow/ast.hpp"
#include "arrow/code.hpp"

namespace arrow {

class Generator;

class Resolver : public ast::AbstractVisitor {
 public:
  explicit Resolver(Generator& g);

  virtual ~Resolver() noexcept;

  std::shared_ptr<code::Type> get() const {
    if (_stack.size() == 0) {
      return nullptr;
    } else {
      return _stack.top();
    }
  }

  virtual void visit(ast::Node&) { }
  virtual void visit(ast::TextNode&) { }
  virtual void visit(ast::Identifier&) { }
  virtual void visit(ast::Function&) { }
  virtual void visit(ast::Module&) { }
  virtual void visit(ast::Call&) { }
  virtual void visit(ast::Break&) { }
  virtual void visit(ast::Slot&) { }
  virtual void visit(ast::Return&) { }
  virtual void visit(ast::Integer&);
  virtual void visit(ast::Float&);
  virtual void visit(ast::Boolean&);
  virtual void visit(ast::String&) { }
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
  virtual void visit(ast::Add&);
  virtual void visit(ast::Sub&);
  virtual void visit(ast::Mul&);
  virtual void visit(ast::Div&);
  virtual void visit(ast::Mod&);

 private:
  Generator& _g;
  std::stack<std::shared_ptr<code::Type>> _stack;
};

extern std::shared_ptr<code::Type> resolve(Generator& _g, ast::Node& x);

}  // namespace arrow

#endif  // ARROW_RESOLVE_H
