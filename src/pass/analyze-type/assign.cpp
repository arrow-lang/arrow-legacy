// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/pass/analyze-type.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

bool AnalyzeType::_expand_assign(
  ast::Assign& node, ast::Node& lhs, Ref<code::Type> type
) {
  Match(lhs) {
    Case(ast::Identifier& x) {
      XTL_UNUSED(x);

      // Check for a declared name
      auto item = _scope->find(x.text);
      if (!item) {
        Log::get().error(
          x.span, "use of unresolved name '%s'", x.text.c_str());

        return false;
      }

      if (item.is<code::Slot>()) {
        // Mark [assign]
        _assign[item.as<code::Slot>().get()].push_back({type});
      }
    } break;

    Case(ast::Tuple& x) {
      if (!type.is<code::TypeTuple>()) {
        // TODO(mehcode): Error
        return false;
      }

      auto type_tuple = type.as<code::TypeTuple>();
      if (type_tuple->elements.size() != x.elements.size()) {
        // TODO(mehcode): Error
        return false;
      }

      unsigned idx = 0;
      for (auto& el : x.elements) {
        _expand_assign(node, *el, type_tuple->elements.at(idx));
        idx += 1;
      }
    } break;

    Otherwise() {
      return false;
    }
  } EndMatch;

  return true;
}

void AnalyzeType::visit_assign(ast::Assign& x) {
  // Resolve the type of the RHS
  auto type = Resolve(_scope).run(*x.rhs);
  if (type && type->is_unknown()) {
    if (Log::get().count("error") == 0) {
      _incomplete = true;
    }

    return;
  }

  // Expand the assignment
  _expand_assign(x, *x.lhs, type);
}

}  // namespace pass
}  // namespace arrow
