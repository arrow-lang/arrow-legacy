// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/type.hpp"
#include "arrow/pass/resolve.hpp"
#include "arrow/pass/analyze-type.hpp"
#include "arrow/match.hpp"

namespace arrow {
namespace pass {

void AnalyzeType::visit_struct(ast::Structure& x) {
  // Pull out the previously-exposed item
  auto item = _scope->find(&x).as<code::Structure>();
  if (!item) return;

  // Iterate through each member and push in a member type
  item->type->members.clear();
  for (auto& mem : x.members) {
    // Attempt to resolve the member type
    auto type = Type(_scope).run(*mem->type);
    if (!type) return;

    // Push the resolved member
    item->type->members.push_back(new code::TypeMember(
      mem->name,
      type
    ));
  }
}

}  // namespace pass
}  // namespace arrow
