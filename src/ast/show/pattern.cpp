// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/ast.hpp"
#include "arrow/ast/show.hpp"

namespace arrow {
namespace ast {

void Show::visit_pattern_wildcard(PatternWildcard& x) {
  _w.StartObject();

  _w.Key("tag");
  _w.String("PatternWildcard");

  _w.Key("span");
  _w.String(x.span.to_string().c_str());

  _w.EndObject();
}

void Show::visit_pattern_identifier(PatternIdentifier& x) {
  _w.StartObject();

  _w.Key("tag");
  _w.String("PatternIdentifier");

  _w.Key("span");
  _w.String(x.span.to_string().c_str());

  _w.Key("text");
  _w.String(x.text.c_str());

  _w.Key("mutable");
  _w.Bool(x.is_mutable);

  _w.EndObject();
}

void Show::visit_pattern_literal(PatternLiteral& x) {
  _w.StartObject();

  _w.Key("tag");
  _w.String("PatternLiteral");

  _w.Key("span");
  _w.String(x.span.to_string().c_str());

  _w.Key("literal");
  x.literal->accept(*this);

  _w.EndObject();
}

void Show::visit_pattern_tuple(PatternTuple& x) {
  _w.StartObject();

  _w.Key("tag");
  _w.String("PatternTuple");

  _w.Key("span");
  _w.String(x.span.to_string().c_str());

  _w.Key("elements");
  _w.StartArray();
  for (auto& elem : x.elements) {
    elem->accept(*this);
  }

  _w.EndArray();

  _w.EndObject();
}

}  // namespace ast
}  // namespace arrow
