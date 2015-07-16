// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <typeinfo>

#include "arrow/ast.hpp"
#include "arrow/ast/show.hpp"

namespace arrow {
namespace ast {

void Show::visit_slot(Slot& x) {
  _w.StartObject();

  _w.Key("tag");
  _w.String("Slot");

  _w.Key("span");
  _w.String(x.span.to_string().c_str());

  _w.Key("exported");
  _w.Bool(x.exported);

  _w.Key("pattern");
  x.pattern->accept(*this);

  _w.Key("type");
  if (x.type) {
    x.type->accept(*this);
  } else {
    _w.Null();
  }

  _w.Key("initializer");
  if (x.initializer) {
    x.initializer->accept(*this);
  } else {
    _w.Null();
  }

  _w.EndObject();
}

void Show::visit_extern_slot(ExternSlot& x) {
  _w.StartObject();

  _w.Key("tag");
  _w.String("ExternSlot");

  _w.Key("span");
  _w.String(x.span.to_string().c_str());

  _w.Key("exported");
  _w.Bool(x.exported);

  _w.Key("name");
  _w.String(x.name.c_str());

  _w.Key("type");
  x.type->accept(*this);

  _w.EndObject();
}

}  // namespace ast
}  // namespace arrow
