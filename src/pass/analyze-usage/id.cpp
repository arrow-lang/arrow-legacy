// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/pass/analyze-usage.hpp"

namespace arrow {
namespace pass {

void AnalyzeUsage::visit_id(ast::Identifier& x) {
  // Check for a declared item (by-name)
  auto item = _scope->find(x.text);
  if (!item) {
    Log::get().error(
      x.span, "use of unresolved name '%s'", x.text.c_str());

    return;
  }

  Match(*item) {
    Case(code::Slot& slot)  {
      XTL_UNUSED(slot);

      // Check for at least one definite assignment
      bool is_assigned = false;
      bool possible = false;
      auto& ba = _assign[_scope->top().get()];
      auto assign_set_ref = ba.find(item->context);
      if (assign_set_ref != ba.end()) {
        for (auto& assignment : assign_set_ref->second) {
          if (assignment.is_definite) {
            is_assigned = true;
            break;
          } else {
            possible = true;
          }
        }
      }

      if (!is_assigned && possible) {
        Log::get().error(
          x.span, "use of possibly uninitialized variable '%s'",
          x.text.c_str());

        return;
      } else if (!is_assigned) {
        Log::get().error(
          x.span, "use of uninitialized variable '%s'", x.text.c_str());

        return;
      }
    } break;
  } EndMatch;
}

}  // namespace pass
}  // namespace arrow
