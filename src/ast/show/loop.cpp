// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/ast.hpp"
#include "arrow/ast/show.hpp"

namespace arrow {
namespace ast {

void Show::visit_loop(Loop& x) {
  _w.StartObject();

  _w.Key("tag");
  _w.String("Loop");

  _w.Key("span");
  _w.String(x.span.to_string().c_str());

  _w.Key("condition");
  if (x.condition) {
    x.condition->accept(*this);
  } else {
    _w.Null();
  }

  _w.Key("block");
  x.block->accept(*this);

  _w.EndObject();
}

void Show::visit_break(Break& x) {
  _w.StartObject();

  _w.Key("tag");
  _w.String("Break");

  _w.Key("span");
  _w.String(x.span.to_string().c_str());

  _w.EndObject();
}

void Show::visit_continue(Continue& x) {
  _w.StartObject();

  _w.Key("tag");
  _w.String("Continue");

  _w.Key("span");
  _w.String(x.span.to_string().c_str());

  _w.EndObject();
}

}  // namespace ast
}  // namespace arrow
