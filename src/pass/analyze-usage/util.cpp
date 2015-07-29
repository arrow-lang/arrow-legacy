// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/pass/analyze-usage.hpp"

namespace arrow {
namespace pass {

void AnalyzeUsage::do_use(ast::Node& context, code::Slot& item) {
  if (!item.is_declared) {
    Log::get().error(
      context.span, "use of unresolved name '%s'",
      item.name.c_str());

    return;
  }

  auto is_assigned = item.is_assigned(_scope->top());

  if ((is_assigned && !(*is_assigned)) || !is_assigned) {
    // If we will error AND we are non-local
    if (!item.is_local(_scope)) {
      // Mark the use and ignore error
      _use.insert(&item);
      return;
    }
  }

  if (is_assigned && !(*is_assigned)) {
    Log::get().error(
      context.span, "use of possibly uninitialized variable '%s'",
      item.name.c_str());

    return;
  } else if (!is_assigned) {
    Log::get().error(
      context.span, "use of uninitialized variable '%s'",
      item.name.c_str());

    return;
  }
}

void AnalyzeUsage::do_assign(
  ast::Node& context, Ref<code::Slot> item, bool is_definite
) {
  // Are we immutable ..
  if (!item->is_mutable) {
    // Are we non-local ..
    if (!item->is_local(_scope)) {
      Log::get().error(
        context.span, "cannot assign immutable non-local variable `%s`",
        item->name.c_str());

      return;
    }

    // Have we been assigned previously ..
    auto is_assigned = item->is_assigned(_scope->top());
    if (is_assigned) {
      Log::get().error(
        context.span, "re-assignment of immutable variable `%s`",
        item->name.c_str());

      return;
    }
  }

  // Mark [assign]
  _assign[_scope->top()].push_back(item);
  item->add_assignment(_scope->top(), true);
}

}  // namespace pass
}  // namespace arrow
