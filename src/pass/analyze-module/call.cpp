// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/util.hpp"
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
  if (function->_dependencies.size() > 0) is_static = false;
  if (is_static && function->_use.size() > 0) {
    for (auto& use : function->_use) {
      if (!use->is_static) {
        is_static = false;
        break;
      }
    }
  }

  // Retrieve what module(s) a function-type originates from
  auto owner = _scope->get_owner();
  auto cur_mod = util::as<code::Module*>(owner);
  auto& modules = function->_modules;

  // NOTE: Do not add the module dependency if the current module
  //       is the only defined module for this function type;
  //       we have other ways of ensuring safety within the module
  if (function->_dependencies.size() == 0 && modules.size() == 1) {
    if (*(modules.begin()) == cur_mod) {
      return;
    }
  }

  if (!is_static) {
    // Used a non-static from another module; record the dependency
    if (util::is<code::Module>(*owner)) {
      cur_mod->dependencies.insert(modules.begin(), modules.end());
      cur_mod->dependencies.insert(
        function->_dependencies.begin(), function->_dependencies.end());
    } else {  // Function
      auto fn = util::as<code::Function*>(owner);
      auto fn_type = fn->type.as<code::TypeFunction>();
      fn_type->_dependencies.insert(modules.begin(), modules.end());
      fn_type->_dependencies.insert(
        function->_dependencies.begin(), function->_dependencies.end());
    }
  }
}

}  // namespace pass
}  // namespace arrow
