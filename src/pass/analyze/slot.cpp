// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/pass/analyze.hpp"
#include "arrow/pass/type.hpp"

namespace arrow {

static bool _expand_pattern(
  ast::Pattern& pattern,
  Ref<code::Scope> scope,
  Ref<code::Type> type_annotation
) {
  Match(pattern) {
    Case(ast::PatternWildcard& x) {
      XTL_UNUSED(x);

      // Do nothing
    } break;

    Case(ast::PatternIdentifier& x) {
      // Pull out the previously-exposed item
      auto item = scope->get(&pattern).as<code::Slot>();
      if (!item) return false;

      // If we have an explicit type annotation ..
      if (type_annotation) {
        // .. mark this slot with the annotated type
        item->type = type_annotation;
      }

      // TODO(mehcode): No explicit annotation
    } break;

    Otherwise() {
      // FIXME(mehcode): Implement
      Log::get().error(
        pattern.span, "not (yet) implemented");

      return false;
    }
  } EndMatch;

  return true;
}

namespace pass {

void Analyze::visit_slot(ast::Slot& x) {
  // Check for an explicit type annotation ..
  Ref<code::Type> type_annotation = nullptr;
  if (x.type) {
    type_annotation = Type(_scope).run(*x.type);
    if (!type_annotation) return;
  }

  // Expand the pattern ..
  _expand_pattern(*x.pattern, _scope, type_annotation);
}

}  // namespace pass
}  // namespace arrow
