// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/pass/expose.hpp"
#include "arrow/pass/type.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void Expose::visit_function(ast::Function& x) {
  // Add this (undefined and un-analyzed) to the current scope
  Ref<code::Function> item = new code::Function(
    /*context=*/&x,
    /*name=*/x.name,
    /*parent_scope=*/_scope);
  _scope->insert(item);

  // If exported; push into the module items
  if (x.exported) {
    auto mod = dynamic_cast<code::Module*>(_scope->get_owner());
    if (mod) {
      mod->items.emplace(x.name, item);
    }
  }

  // Enter the function scope-block
  item->scope->enter(&x);

  // Iterate and emplace parameters (in the function scope)
  for (unsigned param_idx = 0; param_idx < x.parameters.size(); ++param_idx) {
    auto& param = x.parameters.at(param_idx);

    // Expand the parameter
    if (!_expand_parameter_pattern(*param->pattern, item->scope)) {
      return;
    }
  }

  // Expose the function body
  Expose(_ctx, item->scope).run(*x.block);

  // Leave the function scope-block
  item->scope->exit();
}

bool Expose::_expand_parameter_pattern(
  ast::Pattern& pattern, Ref<code::Scope> s
) {
  Match(pattern) {
    Case(ast::PatternWildcard& x) {
      XTL_UNUSED(x);

      // Do nothing
    } break;

    Case(ast::PatternIdentifier& x) {
      // Add this (undefined and un-analyzed) to the current scope
      s->insert(new code::Parameter(
        /*context=*/&pattern,
        /*name=*/x.text,
        /*is_mutable=*/x.is_mutable));
    } break;

    Otherwise() {
      return false;
    }
  } EndMatch;

  return true;
}

}  // namespace pass
}  // namespace arrow
