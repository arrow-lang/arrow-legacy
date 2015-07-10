// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/analyze.hpp"

namespace arrow {
namespace pass {

void Analyze::do_binary(ast::Binary& x) {
  x.lhs->accept(*this);
  x.rhs->accept(*this);
}

void Analyze::visit_add(ast::Add& x)                 { do_binary(x); }
void Analyze::visit_sub(ast::Sub& x)                 { do_binary(x); }
void Analyze::visit_mul(ast::Mul& x)                 { do_binary(x); }
void Analyze::visit_div(ast::Div& x)                 { do_binary(x); }
void Analyze::visit_mod(ast::Mod& x)                 { do_binary(x); }
void Analyze::visit_eq(ast::EqualTo& x)              { do_binary(x); }
void Analyze::visit_ne(ast::NotEqualTo& x)           { do_binary(x); }
void Analyze::visit_lt(ast::LessThan& x)             { do_binary(x); }
void Analyze::visit_le(ast::LessThanOrEqualTo& x)    { do_binary(x); }
void Analyze::visit_ge(ast::GreaterThanOrEqualTo& x) { do_binary(x); }
void Analyze::visit_gt(ast::GreaterThan& x)          { do_binary(x); }
void Analyze::visit_bit_and(ast::BitAnd& x)          { do_binary(x); }
void Analyze::visit_bit_xor(ast::BitXor& x)          { do_binary(x); }
void Analyze::visit_bit_or(ast::BitOr& x)            { do_binary(x); }
void Analyze::visit_and(ast::And& x)                 { do_binary(x); }
void Analyze::visit_or(ast::Or& x)                   { do_binary(x); }

}  // namespace pass
}  // namespace arrow
