// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/pass/analyze-usage.hpp"
#include "arrow/pass/resolve.hpp"
#include "arrow/pass/type.hpp"

namespace arrow {
namespace pass {

bool AnalyzeUsage::_expand_assign(
  ast::Node& lhs, ast::Assign* context
) {
  Match(lhs) {
    Case(ast::Identifier& x) {
      // Check for a declared item (by-name)
      auto item = _scope->find(x.text);
      if (!item) {
        Log::get().error(
          x.span, "use of unresolved name '%s'", x.text.c_str());

        return false;
      }

      Match(*item) {
        Case(code::Slot& slot)  {
          // Are we immutable and have been assigned previously
          auto& ba = _assign[_scope->top().get()];
          if (!slot.is_mutable) {
            auto assign_set_ref = ba.find(item->context);
            if (assign_set_ref != ba.end()) {
              if (assign_set_ref->second.size() > 0) {
                // We have been assigned before (possibly)
                // TODO(mehcode): Check if this is a possible or definite
                //                re-assignment
                Log::get().error(
                  lhs.span, "re-assignment of immutable variable `%s`",
                  x.text.c_str());

                return false;
              }
            }
          }

          // Mark [assign]
          ba[item->context].emplace_back(true);

          // Check if this is a non-local assignment ..
          if (!_scope->top()->contains(item->context, false)) {
            // Mark [non-local-assign]
            _non_local_assign[_scope->top().get()].insert(item->context);
          }
        } break;

        Case(code::ExternSlot& slot) {
          if (!slot.is_mutable) {
            Log::get().error(
              lhs.span, "assignment of immutable, external variable `%s`",
              x.text.c_str());

            return false;
          }
        } break;

        Otherwise() {
          // Pulling out anything else is an illegal assignment
          Log::get().error(lhs.span, "illegal left-hand side expression");
          return false;
        } break;
      } EndMatch;
    } break;

    Case(ast::PatternTuple& x) {
      // Iterate and expand through each element in the pattern-tuple ..
      for (auto& element : x.elements) {
        if (!_expand_assign(*element, context)) {
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

void AnalyzeUsage::visit_assign(ast::Assign& x) {
  // Analyze the assignment operand ..
  x.rhs->accept(*this);

  // Expand the assignment
  if (!_expand_assign(*x.lhs, &x)) return;
}

}  // namespace pass
}  // namespace arrow
