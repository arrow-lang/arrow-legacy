// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/analyze.hpp"

namespace arrow {
namespace pass {

void Analyze::do_unary(ast::Unary& x) {
  x.operand->accept(*this);
}

void Analyze::visit_identity(ast::Identity& x)      { do_unary(x); }
void Analyze::visit_negate(ast::Negate& x)          { do_unary(x); }
void Analyze::visit_bit_not(ast::BitNot& x)         { do_unary(x); }
void Analyze::visit_deref(ast::Dereference& x)      { do_unary(x); }
void Analyze::visit_not(ast::Not& x)                { do_unary(x); }
void Analyze::visit_address_of(ast::AddressOf& x)   { do_unary(x); }

}  // namespace pass
}  // namespace arrow
