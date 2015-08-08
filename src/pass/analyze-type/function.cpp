// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/analyze-type.hpp"
#include "arrow/pass/resolve.hpp"
#include "arrow/match.hpp"
#include "arrow/util.hpp"

namespace arrow {
namespace pass {

void AnalyzeType::visit_function(ast::Function& x) {
  // Pull out the previously-exposed item
  auto item = _scope->find(&x).as<code::Function>();
  if (!item) return;

  bool resolve_params = false;
  Ref<code::TypeFunction> type;
  if (!item->type || item->type->is_unknown()) {
    // Attempt to resolve the type of this ..
    resolve_params = true;
    type = Resolve(_scope).run(x);
    if (type->is_unknown()) {
      _incomplete = true;
      resolve_params = false;
    } else {
      // Place the type on the item
      item->type = type;

      // Add the current module to the type
      auto module = util::current_module(_scope);
      type.as<code::TypeFunction>()->_modules.insert(module);
    }
  } else {
    type = item->type.as<code::TypeFunction>();
  }

  // Enter the function scope-block
  item->scope->enter(&x);

  if (resolve_params) {
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
  }

  // Save the current-scope and set the item-scope
  auto current_scope = _scope;
  _scope = item->scope;

  // Analyze the function body
  x.block->accept(*this);

  // Exit the function scope-block
  item->scope->exit();

  // Restore the previous scope
  _scope = current_scope;
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
