// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/analyze-usage.hpp"
#include "arrow/pass/resolve.hpp"
#include "arrow/pass/type.hpp"
#include "arrow/match.hpp"

namespace arrow {
namespace pass {

bool AnalyzeUsage::_expand_pattern(
  ast::Pattern& pattern, bool has_initializer
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

      // If we have an initializer ..
      if (has_initializer) {
        // Mark [assign]
        _assign[_scope->top()].push_back(item.get());
        item->add_assignment(_scope->top(), true);
      }

      // We are now declared
      item->is_declared = true;
    } break;

    Case(ast::PatternTuple& x) {
      // Iterate and expand through each element in the pattern-tuple ..
      for (auto& element : x.elements) {
        if (!_expand_pattern(*element, true)) {
          return false;
        }
      }
    } break;

    Otherwise() {
      return false;
    }
  } EndMatch;

  return true;
}

void AnalyzeUsage::visit_slot(ast::Slot& x) {
  // Check for and analyze the initializer ..
  if (x.initializer) x.initializer->accept(*this);

  // Expand the pattern ..
  if (!_expand_pattern(*x.pattern, x.initializer != nullptr)) return;

  if (x.initializer) {
    // Resolve the function-type of the operand
    auto type = Resolve(_scope).run(*x.initializer);
    if (!type || !type.is<code::TypeFunction>()) return;
    auto function = type.as<code::TypeFunction>();

    // Realize the function
    do_realize_function(x, *function);
  }
}

}  // namespace pass
}  // namespace arrow
