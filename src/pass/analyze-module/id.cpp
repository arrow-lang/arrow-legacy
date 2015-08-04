// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/pass/analyze-module.hpp"

namespace arrow {
namespace pass {

void AnalyzeModule::visit_id(ast::Identifier& x) {
  // Check for a declared item (by-name)
  auto item = _scope->find(x.text);
  if (!item) return;

  Match(*item) {
    Case(code::Function& function) {
      // Check if we've analyzed the function ..

      auto func_type = function.type.as<code::TypeFunction>();
      if (func_type->_is_module_analyzed) return;

      // Analyze the function context ..
      function.context->accept(*this);
    } break;
  } EndMatch;
}

}  // namespace pass
}  // namespace arrow
