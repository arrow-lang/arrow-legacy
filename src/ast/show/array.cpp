// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/ast.hpp"
#include "arrow/ast/show.hpp"

namespace arrow {
namespace ast {

void Show::visit_array(Array& x) {
  do_("Array", x, [&, this] {
    _w.Key("elements");
    _w.StartArray();

    for (auto& elem : x.elements) {
      elem->accept(*this);
    }

    _w.EndArray();
  });
}

}  // namespace ast
}  // namespace arrow
