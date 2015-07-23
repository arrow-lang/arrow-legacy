// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/pass/type.hpp"
#include "arrow/pass/resolve.hpp"
#include "arrow/pass/analyze-type.hpp"

namespace arrow {
namespace pass {

void AnalyzeType::visit_slot(ast::Slot& x) {
  // Check for an explicit type annotation ..
  Ref<code::Type> type_annotation = nullptr;
  if (x.type) {
    type_annotation = Type(_scope).run(*x.type);
    if (!type_annotation) return;
  }

  // Check for an initializer ..
  Ref<code::Type> type_initializer = nullptr;
  if (x.initializer) {
    // Resolve the type of the initializer
    type_initializer = Resolve(_scope).run(*x.initializer);
    if (type_initializer->is_unknown()) {
      _incomplete = true;
    }
  }

  // Expand the pattern ..
  _expand_pattern(*x.pattern, type_annotation, type_initializer);
}

bool AnalyzeType::_expand_pattern(
  ast::Pattern& pattern,
  Ref<code::Type> type_annotation,
  Ref<code::Type> type_initializer
) {
  Match(pattern) {
    Case(ast::PatternWildcard& x) {
      XTL_UNUSED(x);

      // Do nothing
    } break;

    Case(ast::PatternIdentifier& x) {
      XTL_UNUSED(x);

      // Pull out the previously-exposed item
      auto item = _scope->find(&pattern).as<code::Slot>();
      if (!item) return false;

      // If we have an explicit type annotation ..
      if (type_annotation) {
        // .. mark this slot with the annotated type
        item->type = type_annotation;
      }

      // Mark [decl]
      _assign[&pattern] = {};

      // If we have an initializer ..
      if (type_initializer) {
        // Mark [assign]
        _assign[&pattern].push_back({type_initializer});
      }
    } break;

    Case(ast::PatternTuple& x) {
      if (type_initializer->is_unknown()) {
        // Initializer type is not ready (yet)
        return false;
      }

      // Check to see if we have the exact # of elements
      auto init_type_tuple = type_initializer.as<code::TypeTuple>();
      if (x.elements.size() != init_type_tuple->elements.size()) {
        Log::get().error(x.span,
          "expected a tuple with %d elements, found one with %d elements",
          init_type_tuple->elements.size(),
          x.elements.size());

        return false;
      }

      Ref<code::TypeTuple> annt_type_tuple = nullptr;
      if (type_annotation) {
        annt_type_tuple = type_annotation.as<code::TypeTuple>();
      }

      unsigned index = 0;
      for (auto& element : x.elements) {
        Ref<code::Type> el_type_annotation = nullptr;
        Ref<code::Type> el_type_initializer = nullptr;

        if (type_annotation) {
          el_type_annotation = annt_type_tuple->elements.at(index);
        }

        el_type_initializer = init_type_tuple->elements.at(index);

        if (!_expand_pattern(
            *element, el_type_annotation, el_type_initializer)) {
          return false;
        }

        index += 1;
      }
    } break;

    Otherwise() {
      return false;
    }
  } EndMatch;

  return true;
}

}  // namespace pass
}  // namespace arrow
