// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/ast.hpp"

namespace arrow {
namespace ast {

Visitor::~Visitor() noexcept {
}

void Visitor::run(Node& x) {
  x.accept(*this);
}

// Block (scope)
// -----------------------------------------------------------------------------
void Visitor::visit_block(ast::Block& x) {
  for (auto& stmt : x.statements) {
    stmt->accept(*this);
  }
}

// Return
// -----------------------------------------------------------------------------
void Visitor::visit_return(ast::Return& x) {
  if (x.expression) {
    x.expression->accept(*this);
  }
}

// Select
// -----------------------------------------------------------------------------
void Visitor::visit_select(ast::Select& x) {
  for (auto& br : x.branches) {
    br->accept(*this);
  }

  if (x.else_block) {
    x.else_block->accept(*this);
  }
}

void Visitor::visit_select_branch(ast::SelectBranch& x) {
  x.condition->accept(*this);
  x.block->accept(*this);
}

void Visitor::visit_conditional(ast::Conditional& x) {
  x.condition->accept(*this);
  x.lhs->accept(*this);
  x.rhs->accept(*this);
}

// Loop
// -----------------------------------------------------------------------------
void Visitor::visit_loop(ast::Loop& x) {
  if (x.condition) {
    x.condition->accept(*this);
  }

  x.block->accept(*this);
}

// Call
// -----------------------------------------------------------------------------
void Visitor::visit_call(ast::Call& x) {
  x.operand->accept(*this);

  for (auto& arg : x.arguments) {
    arg->accept(*this);
  }
}

void Visitor::visit_argument(ast::Argument& x) {
  x.expression->accept(*this);
}

// Unary
// -----------------------------------------------------------------------------
void Visitor::do_unary(ast::Unary& x) {
  x.operand->accept(*this);
}

void Visitor::visit_identity(ast::Identity& x)      { do_unary(x); }
void Visitor::visit_negate(ast::Negate& x)          { do_unary(x); }
void Visitor::visit_bit_not(ast::BitNot& x)         { do_unary(x); }
void Visitor::visit_deref(ast::Dereference& x)      { do_unary(x); }
void Visitor::visit_not(ast::Not& x)                { do_unary(x); }
void Visitor::visit_address_of(ast::AddressOf& x)   { do_unary(x); }

// Binary
// -----------------------------------------------------------------------------
void Visitor::do_binary(ast::Binary& x) {
  x.lhs->accept(*this);
  x.rhs->accept(*this);
}

void Visitor::visit_add(ast::Add& x)                 { do_binary(x); }
void Visitor::visit_sub(ast::Sub& x)                 { do_binary(x); }
void Visitor::visit_mul(ast::Mul& x)                 { do_binary(x); }
void Visitor::visit_div(ast::Div& x)                 { do_binary(x); }
void Visitor::visit_mod(ast::Mod& x)                 { do_binary(x); }
void Visitor::visit_eq(ast::EqualTo& x)              { do_binary(x); }
void Visitor::visit_ne(ast::NotEqualTo& x)           { do_binary(x); }
void Visitor::visit_lt(ast::LessThan& x)             { do_binary(x); }
void Visitor::visit_le(ast::LessThanOrEqualTo& x)    { do_binary(x); }
void Visitor::visit_ge(ast::GreaterThanOrEqualTo& x) { do_binary(x); }
void Visitor::visit_gt(ast::GreaterThan& x)          { do_binary(x); }
void Visitor::visit_bit_and(ast::BitAnd& x)          { do_binary(x); }
void Visitor::visit_bit_xor(ast::BitXor& x)          { do_binary(x); }
void Visitor::visit_bit_or(ast::BitOr& x)            { do_binary(x); }
void Visitor::visit_and(ast::And& x)                 { do_binary(x); }
void Visitor::visit_or(ast::Or& x)                   { do_binary(x); }
void Visitor::visit_assign(ast::Assign& x)           { do_binary(x); }
void Visitor::visit_index(ast::Index& x)             { do_binary(x); }

void Visitor::visit_cast(ast::Cast& x) {
  x.lhs->accept(*this);
  x.rhs->accept(*this);
}

void Visitor::visit_path(ast::Path& x) {
  x.operand->accept(*this);
}

}  // namespace ast
}  // namespace arrow
