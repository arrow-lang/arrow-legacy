// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/pass/expose.hpp"

namespace arrow {

static bool _expand_pattern(Ref<code::Scope> s, ast::Pattern& pattern) {
  Match(pattern) {
    Case(ast::PatternWildcard& x) {
      XTL_UNUSED(x);

      // Wildcard pattern would instruct me to do nothing.. nothing at
      // all.. hehe
    } break;

    Case(ast::PatternIdentifier& x) {
      // Add this (undefined and un-analyzed) to the current scope
      s->emplace(new code::Slot(
        /*context=*/&pattern,
        /*name=*/x.text,
        /*is_mutable=*/x.is_mutable));
    } break;

    Case(ast::PatternTuple& x) {
      // A tuple-pattern (in this instance) is just a
      // sequence of identifiers/wildcards to be exposed
      for (auto& elem : x.elements) {
        _expand_pattern(s, *elem);
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
  _expand_pattern(_scope, *x.pattern);
}

}  // namespace pass
}  // namespace arrow
