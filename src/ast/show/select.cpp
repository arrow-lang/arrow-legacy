// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/ast.hpp"
#include "arrow/ast/show.hpp"

namespace arrow {
namespace ast {

void Show::visit_select_branch(Branch& x) {
  do_("SelectBranch", x, [&, this] {
    _w.Key("condition");
    x.condition->accept(*this);

    _w.Key("block");
    x.block->accept(*this);
  });
}

void Show::visit_select(Select& x) {
  do_("Select", x, [&, this] {
    _w.Key("branches");
    _w.StartArray();

    for (auto& br : x.branches) {
      br->accept(*this);
    }

    _w.EndArray();

    _w.Key("else_block");
    if (x.else_block) {
      x.else_block->accept(*this);
    } else {
      _w.Null();
    }
  });
}

}  // namespace ast
}  // namespace arrow
