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
    Case(code::Slot& slot) {
      XTL_UNUSED(slot);

      do_use(x, *(item.as<code::Slot>()));
    } break;

    Case(code::Function& function) {
      // Check if we've analyzed the function ..
      auto func_type = function.type.as<code::TypeFunction>();
      if (func_type->_is_analyzed) return;

      // Analyze the function context ..
      AnalyzeUsage(_ctx, function.scope->parent()).run(*function.context);
    } break;
  } EndMatch;
}

}  // namespace pass
}  // namespace arrow
