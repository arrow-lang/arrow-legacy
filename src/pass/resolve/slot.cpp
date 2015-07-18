// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

static Ref<code::Type> _resolve_pattern(code::Scope& s, ast::Pattern& pattern) {
  Match(pattern) {
    Case(ast::PatternWildcard& x) {
      XTL_UNUSED(x);

      // Do nothing
      return new code::TypeAny();
    } break;

    Case(ast::PatternIdentifier& x) {
      // Pull out the previously-exposed item
      // TODO(mehcode): `scope->get<T>`
      auto item = s.get(&x).as<code::Slot>();
      if (!item || !item->type) return nullptr;

      // Return its bound type
      return item->type;
    } break;

    Case(ast::PatternTuple& x) {
      // Create the initial type
      Ref<code::TypeTuple> tuple = new code::TypeTuple();

      // Iterate through the elements; and resolve each
      tuple->elements.reserve(x.elements.size());
      for (auto& element : x.elements) {
        auto element_type = _resolve_pattern(s, *element);
        if (!element_type && !element_type.is_undef()) return nullptr;

        tuple->elements.push_back(element_type);
      }

      return tuple;
    } break;

    Otherwise() {
      // Unreachable
      return nullptr;
    }
  } EndMatch;

  // Unreachable
  return nullptr;
}

void Resolve::visit_slot(ast::Slot& x) {
  auto type = _resolve_pattern(*_scope, *x.pattern);
  if (type) {
    _stack.push_front(type);
  }
}

}  // namespace pass
}  // namespace arrow
