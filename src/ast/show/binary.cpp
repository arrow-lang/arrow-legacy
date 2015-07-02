// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/ast.hpp"
#include "arrow/ast/show.hpp"

namespace arrow {
namespace ast {

void Show::do_binary(const char* name, Binary& x) {
  _w.StartObject();

  _w.Key("tag");
  _w.String(name);

  _w.Key("span");
  _w.String(x.span.to_string().c_str());

  _w.Key("lhs");
  x.lhs->accept(*this);

  _w.Key("rhs");
  x.rhs->accept(*this);

  _w.EndObject();
}

void Show::visit_add(Add& x)         { do_binary("Add",    x); }
void Show::visit_sub(Sub& x)         { do_binary("Sub",    x); }
void Show::visit_mul(Mul& x)         { do_binary("Mul",    x); }
void Show::visit_div(Div& x)         { do_binary("Div",    x); }
void Show::visit_mod(Mod& x)         { do_binary("Mod",    x); }

void Show::visit_eq(EqualTo& x)      { do_binary("EqualTo",     x); }
void Show::visit_ne(NotEqualTo& x)   { do_binary("NotEqualTo",  x); }
void Show::visit_gt(GreaterThan& x)  { do_binary("GreaterThan", x); }
void Show::visit_lt(LessThan& x)     { do_binary("LessThan",    x); }

void Show::visit_le(LessThanOrEqualTo& x) {
  do_binary("LessThanOrEqualTo", x);
}

void Show::visit_ge(GreaterThanOrEqualTo& x) {
  do_binary("GreaterThanOrEqualTo", x);
}

void Show::visit_bit_and(BitAnd& x)  { do_binary("BitAnd", x); }
void Show::visit_bit_or(BitOr& x)    { do_binary("BitOr",  x); }
void Show::visit_bit_xor(BitXor& x)  { do_binary("BitXor", x); }

void Show::visit_and(And& x)         { do_binary("And", x); }
void Show::visit_or(Or& x)           { do_binary("Or",  x); }

void Show::visit_assign(Assign& x)   { do_binary("Assign", x); }
void Show::visit_cast(Cast& x)       { do_binary("Cast", x); }

void Show::visit_path(Path& x) {
  do_("Path", x, [&, this] {
    _w.Key("operand");
    x.operand->accept(*this);

    _w.Key("member");
    _w.String(x.member.c_str());
  });
}

}  // namespace ast
}  // namespace arrow
