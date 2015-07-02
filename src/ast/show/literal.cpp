// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/ast.hpp"
#include "arrow/ast/show.hpp"

namespace arrow {
namespace ast {

void Show::visit_id(Identifier& x) {
  _w.StartObject();

  _w.Key("tag");
  _w.String("Identifier");

  _w.Key("span");
  _w.String(x.span.to_string().c_str());

  _w.Key("text");
  _w.String(x.text.c_str());

  _w.EndObject();
}

void Show::visit_none(None& x) {
  do_("None", x);
}

void Show::visit_int(Integer& x) {
  _w.StartObject();

  _w.Key("tag");
  _w.String("Integer");

  _w.Key("span");
  _w.String(x.span.to_string().c_str());

  _w.Key("text");
  _w.String(x.text.c_str());

  _w.EndObject();
}

void Show::visit_float(Float& x) {
  _w.StartObject();

  _w.Key("tag");
  _w.String("Float");

  _w.Key("span");
  _w.String(x.span.to_string().c_str());

  _w.Key("text");
  _w.String(x.text.c_str());

  _w.EndObject();
}

void Show::visit_str(String& x) {
  _w.StartObject();

  _w.Key("tag");
  _w.String("String");

  _w.Key("span");
  _w.String(x.span.to_string().c_str());

  _w.Key("text");
  _w.String(x.text.c_str());

  _w.EndObject();
}

void Show::visit_bool(Boolean& x) {
  _w.StartObject();

  _w.Key("tag");
  _w.String("Boolean");

  _w.Key("span");
  _w.String(x.span.to_string().c_str());

  _w.Key("value");
  _w.Bool(x.value);

  _w.EndObject();
}

}  // namespace ast
}  // namespace arrow
