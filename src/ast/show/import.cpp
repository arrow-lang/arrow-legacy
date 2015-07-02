// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/ast.hpp"
#include "arrow/ast/show.hpp"

namespace arrow {
namespace ast {

void Show::visit_import(Import& x) {
  do_("Import", x, [&, this] {
    _w.Key("name");
    _w.String(x.name.c_str());

    _w.Key("source");
    _w.String(x.source.c_str());
  });
}

}  // namespace ast
}  // namespace arrow
