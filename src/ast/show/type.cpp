// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/ast.hpp"
#include "arrow/ast/show.hpp"

namespace arrow {
namespace ast {

void Show::visit_type_none(TypeNone& x) {
  do_("TypeNone", x);
}

void Show::visit_type_path(TypePath& x) {
  do_("TypePath", x, [&, this] {
    _w.Key("segments");
    _w.StartArray();

    for (auto& seg : x.segments) {
      _w.String(seg.c_str());
    }

    _w.EndArray();
  });
}

void Show::visit_type_pointer(TypePointer& x) {
  do_("TypePointer", x, [&, this] {
    _w.Key("mutable");
    _w.Bool(x.is_mutable);

    _w.Key("pointee");
    x.pointee->accept(*this);
  });
}

void Show::visit_type_of(TypeOf& x) {
  do_("TypeOf", x, [&, this] {
    _w.Key("expression");
    x.expression->accept(*this);
  });
}

void Show::visit_type_array(TypeArray& x) {
  do_("TypeArray", x, [&, this] {
    _w.Key("element");
    x.element->accept(*this);

    _w.Key("size");
    if (x.size) x.size->accept(*this);
    else _w.Null();
  });
}

void Show::visit_type_tuple(TypeTuple& x) {
  do_("TypeTuple", x, [&, this] {
    _w.Key("elements");
    _w.StartArray();

    for (auto& elem : x.elements) {
      elem->accept(*this);
    }

    _w.EndArray();
  });
}

void Show::visit_type_parameter(TypeParameter& x) {
  do_("TypeParameter", x, [&, this] {
    _w.Key("keyword");
    _w.String(x.keyword.c_str());

    _w.Key("type");
    x.type->accept(*this);
  });
}

void Show::visit_type_function(TypeFunction& x) {
  do_("TypeFunction", x, [&, this] {
    _w.Key("parameters");
    _w.StartArray();

    for (auto& param : x.parameters) {
      param->accept(*this);
    }

    _w.EndArray();

    _w.Key("result");
    x.result->accept(*this);
  });
}

}  // namespace ast
}  // namespace arrow
