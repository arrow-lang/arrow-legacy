// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/pass/analyze.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

bool Analyze::_expand_assign(
  ast::Assign& node, ast::Node& lhs, Ref<code::Type> type
) {
  Match(lhs) {
    Case(ast::Identifier& x) {
      XTL_UNUSED(x);

      // Check for a declared name
      auto ref = _x_name.find(x.text);
      if (ref == _x_name.end()) {
        Log::get().error(
          x.span, "use of unresolved name '%s'", x.text.c_str());

        return false;
      }

      if (_x_assign.find(ref->second) == _x_assign.end()) {
        // Initialize the assignment set with an empty vector
        _x_assign[ref->second] = {};
      }

      // Are we immutable and have been assigned previously ..
      auto& decl = _x_declare[ref->second];
      auto& assign_set = _x_assign[ref->second];
      if (assign_set.size() > 0 && !decl.is_mutable && !decl.is_external) {
        Log::get().error(
          node.span, "re-assignment of immutable variable `%s`",
          x.text.c_str());

        return false;
      }

      // TODO(mehcode): Have we been assigned and not used?

      // Emplace the assignment
      assign_set.push_back({
        true,
        0,
        type
      });
    } break;

    Case(ast::Tuple& x) {
      if (!type.is<code::TypeTuple>()) {
        // TODO(mehcode): Error?
        return false;
      }

      auto type_tuple = type.as<code::TypeTuple>();
      if (type_tuple->elements.size() != x.elements.size()) {
        // TODO(mehcode): Error?
        return false;
      }


      unsigned idx = 0;
      for (auto& el : x.elements) {
        _expand_assign(node, *el, type_tuple->elements.at(idx));
        idx += 1;
      }
    } break;

    Otherwise() {
      Log::get().error(node.span, "not yet implemented");
      return false;
    }
  } EndMatch;

  return true;
}

void Analyze::visit_assign(ast::Assign& x) {
  // Resolve the type of the RHS
  auto type = Resolve(_scope).run(*x.rhs);
  if (!type) {
    if (Log::get().count("error") == 0) {
      _incomplete = true;
    }

    return;
  }

  // Expand the assignment
  if (!_expand_assign(x, *x.lhs, type)) return;

  // Resolve the type of the LHS
  auto lhs_type = Resolve(_scope).run(*x.lhs);
  if (!lhs_type) {
    if (Log::get().count("error") == 0) {
      _incomplete = true;
    }

    return;
  }

  // Check for mismatched types
  // TODO(mehcode): Should be a util for this regardless
  // TODO(mehcode): Should check for `compatible` types (not `equals`)
  if (!lhs_type->equals(*type)) {
    Log::get().error(x.rhs->span,
      "mismatched types: expected `%s`, found `%s`",
      lhs_type->name().c_str(),
      type->name().c_str());

    return;
  }
}

}  // namespace pass
}  // namespace arrow
