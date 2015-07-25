// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void Resolve::visit_select_branch(ast::SelectBranch& x) {
  // Resolve (and push) the type of the block
  auto type = Resolve(_scope).run(*x.block);
  if (type) {
    _stack.push_front(type);
  }
}

void Resolve::visit_select(ast::Select& x) {
  // Iterate and resolve each branch into a list of possible types
  std::vector<Ref<code::Type>> types;

  for (auto& br : x.branches) {
    auto type = Resolve(_scope).run(*br);
    if (type) {
      types.push_back(type);
    }
  }

  // Resolve the else-block (if present)
  if (x.else_block) {
    auto type = Resolve(_scope).run(*x.else_block);
    if (type) {
      types.push_back(type);
    }
  }

  // If we have enough matched types ..
  if (types.size() == x.branches.size() + 1) {
    // Intersect the matched types ..
    auto final_type = code::instersect_all(types);
    if (final_type) {
      _stack.push_front(final_type);
    }
  }
}

}  // namespace pass
}  // namespace arrow
