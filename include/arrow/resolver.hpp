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

class Resolver : public ast::Visitor {
 public:
  Resolver(Generator& g, code::Scope& _scope);

  virtual ~Resolver() noexcept;

  std::shared_ptr<code::Type> get() const {
    if (_stack.size() == 0) {
      return nullptr;
    } else {
      return _stack.top();
    }
  }

  virtual void visit_id(ast::Identifier&);
  virtual void visit_function(ast::Function&);
  virtual void visit_extern_function(ast::ExternalFunction&);
  virtual void visit_call(ast::Call&);
  virtual void visit_int(ast::Integer&);
  virtual void visit_float(ast::Float&);
  virtual void visit_bool(ast::Boolean&);
  virtual void visit_str(ast::String&);
  virtual void visit_promote(ast::Promote&);
  virtual void visit_negate_logical(ast::NegateLogical&);
  virtual void visit_negate_bit(ast::NegateBit&);
  virtual void visit_negate_numeric(ast::NegateNumeric&);
  virtual void visit_assign(ast::Assign&);
  virtual void visit_and(ast::And&);
  virtual void visit_or(ast::Or&);
  virtual void visit_eq(ast::EqualTo&);
  virtual void visit_ne(ast::NotEqualTo&);
  virtual void visit_lt(ast::LessThan&);
  virtual void visit_le(ast::LessThanOrEqualTo&);
  virtual void visit_ge(ast::GreaterThanOrEqualTo&);
  virtual void visit_gt(ast::GreaterThan&);
  virtual void visit_bit_and(ast::BitAnd&);
  virtual void visit_bit_xor(ast::BitXor&);
  virtual void visit_bit_or(ast::BitOr&);
  virtual void visit_add(ast::Add&);
  virtual void visit_sub(ast::Sub&);
  virtual void visit_mul(ast::Mul&);
  virtual void visit_div(ast::Div&);
  virtual void visit_mod(ast::Mod&);
  virtual void visit_cast(ast::Cast&);
  virtual void visit_select(ast::Select&);
  virtual void visit_pointer_type(ast::PointerType&);
  virtual void visit_address_of(ast::AddressOf&);
  virtual void visit_dereference(ast::Dereference&);
  virtual void visit_struct(ast::Structure&);
  virtual void visit_block(ast::Block&);
  virtual void visit_member(ast::Member&);

  // FIXME: Ignored but resolved (for some reason; stop it)
  virtual void visit_return(ast::Return&) { }

  std::shared_ptr<code::Type> common_type(
    std::shared_ptr<ast::Node> lhs,
    std::shared_ptr<ast::Node> rhs);

 private:
  void do_arithmetic(ast::Binary& x);
  void do_function(ast::AbstractFunction& x);
  void do_bitwise(ast::Binary& x);
  void do_relational(ast::Binary& x);
  void do_logical(ast::Binary& x);

  std::shared_ptr<code::Item> do_identifier(ast::Identifier& x);

  Generator& _g;
  code::Scope& _scope;
  std::stack<std::shared_ptr<code::Type>> _stack;
};

extern std::shared_ptr<code::Type> resolve(
  Generator& g, code::Scope& scope, ast::Node& x);

extern std::shared_ptr<code::Type> common_type(
  Generator& g, code::Scope& scope,
  std::shared_ptr<ast::Node> lhs,
  std::shared_ptr<ast::Node> rhs);

}  // namespace arrow

#endif  // ARROW_RESOLVE_H
