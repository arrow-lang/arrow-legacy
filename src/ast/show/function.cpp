// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/ast.hpp"
#include "arrow/ast/show.hpp"

namespace arrow {
namespace ast {

void Show::visit_parameter(Parameter& x) {
  do_("Parameter", x, [&, this] {
    _w.Key("pattern");
    x.pattern->accept(*this);

    _w.Key("type");
    if (x.type) {
      x.type->accept(*this);
    } else {
      _w.Null();
    }

    _w.Key("default_value");
    if (x.default_value) {
      x.default_value->accept(*this);
    } else {
      _w.Null();
    }
  });
}

void Show::visit_function(Function& x) {
  do_("Function", x, [&, this] {
    _w.Key("name");
    _w.String(x.name.c_str());

    _w.Key("exported");
    _w.Bool(x.exported);

    _w.Key("parameters");
    _w.StartArray();

    for (auto& param : x.parameters) {
      param->accept(*this);
    }

    _w.EndArray();

    _w.Key("result_type");
    x.result_type->accept(*this);

    _w.Key("block");
    x.block->accept(*this);
  });
}

void Show::visit_extern_function(ExternFunction& x) {
  do_("ExternFunction", x, [&, this] {
    _w.Key("name");
    _w.String(x.name.c_str());

    _w.Key("exported");
    _w.Bool(x.exported);

    _w.Key("parameters");
    _w.StartArray();

    for (auto& param : x.parameters) {
      param->accept(*this);
    }

    _w.EndArray();

    _w.Key("result_type");
    x.result_type->accept(*this);
  });
}

void Show::visit_return(Return& x) {
  do_("Return", x, [&, this] {
    _w.Key("expression");
    if (x.expression) {
      x.expression->accept(*this);
    } else {
      _w.Null();
    }
  });
}


}  // namespace ast
}  // namespace arrow
