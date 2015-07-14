// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/pass/analyze.hpp"
#include "arrow/pass/resolve.hpp"
#include "arrow/pass/type.hpp"

namespace arrow {

static bool _expand_pattern(
  ast::Pattern& pattern,
  Ref<code::Scope> scope,
  Ref<code::Type> type_annotation,
  Ref<ast::Node> ctx_annotation,
  Ref<code::Type> type_initializer,
  Ref<ast::Node> ctx_initializer
) {
  Match(pattern) {
    Case(ast::PatternWildcard& x) {
      XTL_UNUSED(x);

      // Do nothing
    } break;

    Case(ast::PatternIdentifier& x) {
      XTL_UNUSED(x);

      // Pull out the previously-exposed item
      auto item = scope->get(&pattern).as<code::Slot>();
      if (!item) return false;

      // If we have an explicit type annotation ..
      if (type_annotation) {
        // .. mark this slot with the annotated type
        item->type = type_annotation;
      } else if (type_initializer) {
        // .. else if we have an initializer type
        item->type = type_initializer;
      }

      // TODO(mehcode): No explicit annotation
    } break;

    Case(ast::PatternTuple& x) {
      // Check to see if the initializer is a tuple
      if (!type_initializer.is<code::TypeTuple>()) {
        Log::get().error(ctx_initializer->span,
          "expected a %d-element tuple", x.elements.size());

        return false;
      } else if (!type_annotation.is<code::TypeTuple>()) {
        Log::get().error(ctx_annotation->span,
          "expected a %d-element tuple", x.elements.size());

        return false;
      }

      // Check to see if we have the exact # of elements
      auto type_tuple_init = type_initializer.as<code::TypeTuple>();
      auto type_tuple_annt = type_annotation.as<code::TypeTuple>();
      if (x.elements.size() != type_tuple_init->elements.size()) {
        Log::get().error(
          x.span, "expected a %d-element tuple",
          type_tuple_init->elements.size());

        return false;
      } else if (x.elements.size() != type_tuple_annt->elements.size()) {
        Log::get().error(
          ctx_initializer->span, "expected a %d-element tuple",
          type_tuple_init->elements.size());

        return false;
      }

      unsigned index = 0;
      for (auto& element : x.elements) {
        Ref<code::Type> el_type_annotation = nullptr;
        Ref<ast::Node> el_ctx_annotation = nullptr;
        Ref<code::Type> el_type_initializer = nullptr;
        Ref<ast::Node> el_ctx_initializer = nullptr;

        if (type_annotation && ctx_annotation) {
          el_type_annotation = type_tuple_annt->elements.at(index);
          el_ctx_annotation =
            ctx_annotation.as<ast::TypeTuple>()->elements.at(index);
        }

        if (type_initializer && ctx_initializer) {
          el_type_initializer = type_tuple_init->elements.at(index);
          el_ctx_initializer =
            ctx_initializer.as<ast::Tuple>()->elements.at(index);
        }

        if (!_expand_pattern(
            *element, scope,
            el_type_annotation, el_ctx_annotation,
            el_type_initializer, el_ctx_initializer)) {
          return false;
        }

        index += 1;
      }
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

  // Check for an initializer ..
  Ref<code::Type> type_initializer = nullptr;
  if (x.initializer) {
    type_initializer = Resolve(_scope).run(*x.initializer);
    if (!type_initializer) return;
  }

  // Expand the pattern ..
  _expand_pattern(
    *x.pattern, _scope,
    type_annotation, x.type,
    type_initializer, x.initializer);
}

}  // namespace pass
}  // namespace arrow
