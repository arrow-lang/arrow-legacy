// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/analyze.hpp"

namespace arrow {
namespace pass {

void Analyze::run(ast::Node& x) {
  do {
    // Clear slate
    _incomplete = false;
    _x_declare.clear();
    _x_assign.clear();
    _x_name.clear();

    // Run the base visitor
    ast::Visitor::run(x);

    // Enumerate through each decl. and ensure that all types have been
    // annotated through context
    for (auto& decl : _x_declare) {
      if (decl.second.is_external) continue;

      auto item = _scope->get(decl.first).as<code::Slot>();
      if (!item->type) {
        if (_x_assign.find(decl.first) != _x_assign.end()) {
          auto& assign_set = _x_assign[decl.first];
          // FIXME: Reduce the COMMON type among all times
          item->type = assign_set[0].type;
        }
      }
    }

    if (Log::get().count("error") > 0) break;
  } while (_incomplete);
}

}  // namespace pass
}  // namespace arrow
