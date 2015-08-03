// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/util.hpp"
#include "arrow/pass/expose.hpp"

namespace arrow {

static bool _expand_pattern(Ref<code::Scope> s, ast::Pattern& pattern,
                            bool exported,
                            bool is_constant,
                            bool is_static) {
  Match(pattern) {
    Case(ast::PatternWildcard& x) {
      XTL_UNUSED(x);

      // Wildcard pattern would instruct me to do nothing.. nothing at
      // all.. hehe
    } break;

    Case(ast::PatternIdentifier& x) {
      // Add this (undefined and un-analyzed) to the current scope
      Ref<code::Item> item = new code::Slot(
        &pattern,
        x.text,
        x.is_mutable,
        is_constant,
        is_static);

      s->insert(item);

      // If exported; push into the module items
      if (exported) {
        auto mod = dynamic_cast<code::Module*>(s->get_owner());
        if (mod) {
          mod->items.emplace(x.text, item);
        }
      }
    } break;

    Case(ast::PatternTuple& x) {
      // A tuple-pattern (in this instance) is just a
      // sequence of identifiers/wildcards to be exposed
      for (auto& elem : x.elements) {
        _expand_pattern(s, *elem, exported, is_constant, is_static);
      }
    } break;

    Otherwise() {
      // FIXME(mehcode): Better error message
      Log::get().error(
        pattern.span, "local binding must contain only irrefutable patterns");

      return false;
    }
  } EndMatch;

  return true;
}

namespace pass {

void Expose::visit_slot(ast::Slot& x) {
  // Check if we have an initializer and it is static
  bool is_constant = !!x.initializer;
  bool is_static = x.initializer ? util::is_static(*x.initializer) : false;

  // Expand slot pattern
  _expand_pattern(_scope, *x.pattern, x.exported, is_constant, is_static);
}

}  // namespace pass
}  // namespace arrow
