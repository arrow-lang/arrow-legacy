// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/ast.hpp"
#include "arrow/ast/show.hpp"

namespace arrow {
namespace ast {

void Show::do_unary(const char* name, Unary& x) {
  _w.StartObject();

  _w.Key("tag");
  _w.String(name);

  _w.Key("span");
  _w.String(x.span.to_string().c_str());

  _w.Key("operand");
  x.operand->accept(*this);

  _w.EndObject();
}

void Show::visit_identity(Identity& x)   { do_unary("Identity",    x); }
void Show::visit_negate(Negate& x)       { do_unary("Negate",      x); }
void Show::visit_not(Not& x)             { do_unary("Not",         x); }
void Show::visit_bit_not(BitNot& x)      { do_unary("BitNot",      x); }
void Show::visit_deref(Dereference& x)   { do_unary("Dereference", x); }

void Show::visit_address_of(AddressOf& x) {
  _w.StartObject();

  _w.Key("tag");
  _w.String("AddressOf");

  _w.Key("span");
  _w.String(x.span.to_string().c_str());

  _w.Key("mutable");
  _w.Bool(x.is_mutable);

  _w.Key("operand");
  x.operand->accept(*this);

  _w.EndObject();
}

}  // namespace ast
}  // namespace arrow
