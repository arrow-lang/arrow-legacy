// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/ast.hpp"
#include "arrow/ast/show.hpp"

namespace arrow {
namespace ast {

void Show::visit_member(Member& x) {
  do_("Member", x, [&, this] {
    _w.Key("name");
    _w.String(x.name.c_str());

    _w.Key("type");
    x.type->accept(*this);

    _w.Key("default_value");
    if (x.default_value) {
      x.default_value->accept(*this);
    } else {
      _w.Null();
    }
  });
}

void Show::visit_struct(Structure& x) {
  do_("Structure", x, [&, this] {
    _w.Key("name");
    _w.String(x.name.c_str());

    _w.Key("exported");
    _w.Bool(x.exported);

    _w.Key("members");
    _w.StartArray();

    for (auto& mem : x.members) {
      mem->accept(*this);
    }

    _w.EndArray();
  });
}

}  // namespace ast
}  // namespace arrow
