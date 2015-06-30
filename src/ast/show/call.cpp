// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/ast.hpp"
#include "arrow/ast/show.hpp"

namespace arrow {
namespace ast {

void Show::visit_call(Call& x) {
  _w.StartObject();

  _w.Key("tag");
  _w.String("Call");

  _w.Key("span");
  _w.String(x.span.to_string().c_str());

  _w.Key("arguments");
  _w.StartArray();

  for (auto& arg : x.arguments) {
    arg->accept(*this);
  }

  _w.EndArray();

  _w.EndObject();
}

void Show::visit_argument(Argument& x) {
  _w.StartObject();

  _w.Key("tag");
  _w.String("Argument");

  _w.Key("span");
  _w.String(x.span.to_string().c_str());

  _w.Key("expression");
  x.expression->accept(*this);

  _w.Key("name");
  if (x.name == "") {
    _w.Null();
  } else {
    _w.String(x.name.c_str());
  }

  _w.EndObject();
}

}  // namespace ast
}  // namespace arrow
