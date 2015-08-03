// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/pass/analyze-module.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void AnalyzeModule::visit_call(ast::Call& x) {
  // Run the base method (analyze the operand and each argument)
  Visitor::visit_call(x);
  if (Log::get().count("error") > 0) return;

  // Resolve the function-type of the operand
  auto type = Resolve(_scope).run(*x.operand);
  if (!type || !type.is<code::TypeFunction>()) return;
  auto function = type.as<code::TypeFunction>();

  // A function call is considered static
  // if it assigns, has non-static uses, or has dependencies
  bool is_static = true;
  if (function->_assign.size() > 0) is_static = false;
  if (function->dependencies.size() > 0) is_static = false;
  if (is_static && function->_use.size() > 0) {
    for (auto& use : function->_use) {
      if (!use->is_static) {
        is_static = false;
        break;
      }
    }
  }

  // TODO(_): Record what module(s) a function-type originates from

  // if (!is_static) {
  //   // Used a non-static from another module; record the dependency
  //   auto owner = _scope->get_owner();
  //   if (util::is<code::Module>(*owner)) {
  //     auto current = util::as<code::Module*>(owner);
  //     current->dependencies.insert(mod);
  //   } else {  // Function
  //     auto fn = util::as<code::Function*>(owner);
  //     auto fn_type = fn->type.as<code::TypeFunction>();
  //     fn_type->dependencies.insert(mod.get());
  //   }
  // }
}

}  // namespace pass
}  // namespace arrow
