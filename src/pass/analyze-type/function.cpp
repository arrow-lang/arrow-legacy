// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/analyze-type.hpp"
#include "arrow/pass/resolve.hpp"
#include "arrow/match.hpp"

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

  // Enter the function scope-block
  item->scope->enter(&x);

  // Iterate and set the parameters' types (in the function scope)
  for (unsigned param_idx = 0; param_idx < x.parameters.size(); ++param_idx) {
    auto& param = x.parameters.at(param_idx);
    auto param_type = type.as<code::TypeFunction>()->parameters.at(param_idx);

    // Expand the parameter
    if (!_expand_parameter_pattern(
        *param->pattern, param_type->type, item->scope)) {
      return;
    }
  }

  // Create a child analyzer
  auto child = AnalyzeType(item->scope);

  // Analyze the function body
  child.run(*x.block);

  // Exit the function scope-block
  item->scope->exit();
}

bool AnalyzeType::_expand_parameter_pattern(
  ast::Pattern& pattern, Ref<code::Type> type, Ref<code::Scope> scope
) {
  Match(pattern) {
    Case(ast::PatternWildcard& x) {
      XTL_UNUSED(x);

      // Do nothing
    } break;

    Case(ast::PatternIdentifier& x) {
      XTL_UNUSED(x);

      // Pull out the previously-exposed item
      auto item = scope->find(&pattern).as<code::Parameter>();
      if (!item) return false;

      // Set it's type
      item->type = type;
    } break;

    Otherwise() {
      return false;
    }
  } EndMatch;

  return true;
}

}  // namespace pass
}  // namespace arrow
