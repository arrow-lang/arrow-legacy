// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/resolver.hpp"
#include "arrow/log.hpp"

using arrow::Resolver;
using arrow::resolve;

void Resolver::visit_select(ast::Select& x) {
  // NOTE: If resolve(..) is called than we know someone is requesting
  //  a value from us

  // Push a new scope level
  code::Scope scope{"", &_scope};

  unsigned index = 0;
  bool has_else = false;
  bool has_value = true;
  std::shared_ptr<code::Type> type = nullptr;
  std::shared_ptr<ast::Node> prev_end = nullptr;
  for (; index < x.branches.size(); ++index) {
    auto& br = x.branches.at(index);

    // The last branch (else) is signaled by having no condition.
    if (!br->condition) { has_else = true; }

    // If this is an empty sequence; we no longer have a value
    if (br->sequence.empty()) { has_value = false; break; }

    // Grab the last node in the sequence and resolve against
    // a previous branch
    auto final_node = br->sequence.back();
    auto final_ty = resolve(_g, scope, *final_node);
    if (!final_ty) continue;
    if (!type) {
      type = final_ty;
    } else {
      type = arrow::common_type(_g, scope, prev_end, final_node);
      if (!type) { return; }
    }
    prev_end = final_node;
  }

  if (type && has_value && has_else) {
    _stack.push(type);
  }
}
