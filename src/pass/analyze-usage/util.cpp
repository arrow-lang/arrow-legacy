// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/pass/analyze-usage.hpp"

namespace arrow {
namespace pass {

void AnalyzeUsage::do_use(ast::Node& context, code::Slot& item) {
  // If this a local variable ..
  if (item.is_local(_scope)) {
    // Check for errors ..
    // Is it declared (yet) ..
    if (!item.is_declared) {
      Log::get().error(
        context.span, "use of variable '%s' before declaration",
        item.name.c_str());

      return;
    }

    // Check for uninitialized errors
    auto is_assigned = item.is_assigned(_scope->top());
    if (is_assigned && !(*is_assigned)) {
      Log::get().error(
        context.span, "use of possibly uninitialized variable '%s'",
        item.name.c_str());

      return;
    } else if (!is_assigned) {
      Log::get().error(
        context.span, "use of possibly uninitialized variable '%s'",
        item.name.c_str());

      return;
    }
  } else {
    // Mark the use ..
    _use.insert(&item);
  }
}

void AnalyzeUsage::do_assign(
  ast::Node& context, code::Slot* item, bool is_definite
) {
  // If we are local .. determine errors
  if (item->is_local(_scope)) {
    // Are we declared ..
    if (!item->is_declared) {
      Log::get().error(
        context.span, "use of variable '%s' before declaration",
        item->name.c_str());

      return;
    }

    // Are we immutable ..
    if (!item->is_mutable) {
      // Have we been assigned previously ..
      auto is_assigned = item->is_assigned(_scope->top());
      if (is_assigned) {
        Log::get().error(
          context.span, "re-assignment of immutable variable `%s`",
          item->name.c_str());

        return;
      }
    }
  } else if (!item->is_mutable) {
    Log::get().error(
      context.span, "cannot assign immutable non-local variable `%s`",
      item->name.c_str());

    return;
  }

  // Mark [assign]
  _assign[_scope->top()].push_back(item);
  item->add_assignment(_scope->top(), true);
}

}  // namespace pass
}  // namespace arrow
