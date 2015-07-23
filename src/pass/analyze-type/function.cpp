// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/analyze-type.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void AnalyzeType::visit_function(ast::Function& x) {
  // Pull out the previously-exposed item
  auto item = _scope->find(&x).as<code::Function>();
  if (!item) return;

  // Attempt to resolve the type of this ..
  auto type = Resolve(_scope).run(x);
  if (type->is_unknown()) {
    _incomplete = true;
  } else {
    // Place the type on the item
    item->type = type;
  }

  // TODO(mehcode): Analyze the function /block/
}

}  // namespace pass
}  // namespace arrow
