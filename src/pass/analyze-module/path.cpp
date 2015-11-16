// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/analyze-module.hpp"
#include "arrow/util.hpp"
#include "arrow/match.hpp"

namespace arrow {
namespace pass {

void AnalyzeModule::visit_path(ast::Path& x) {
  // Check if we're running the `.` operator on an identifier [..]
  if (x.operand.is<ast::Identifier>()) {
    // Lookup this identifier and check if its a module [..]
    auto text = x.operand.as<ast::Identifier>()->text;
    auto item = _scope->find(text);
    if (item.is<code::Import>()) {
      auto imp = item.as<code::Import>();
      auto mod = imp->module;
      auto member = mod->items[x.member];

      bool is_static_use = false;
      bool use = false;
      Match(*member) {
        Case(code::Slot& slot) {
          XTL_UNUSED(slot);

          is_static_use = slot.is_static;
          use = true;
        } break;

        Case(code::ExternSlot& slot) {
          XTL_UNUSED(slot);

          is_static_use = true;
          use = true;
        } break;

        Case(code::Function& fn) {
          auto func_type = fn.type.as<code::TypeFunction>();
          if (!func_type->_is_module_analyzed) {
            // Analyze the function context ..
            AnalyzeModule(_ctx, mod->scope).run(*fn.context);
          }
        } break;
      } EndMatch;

      if (use && !is_static_use) {
        // Used a non-static from another module; record the dependency
        auto owner = _scope->get_owner();
        if (util::is<code::Module>(*owner)) {
          auto current = util::as<code::Module*>(owner);
          current->dependencies.insert(mod.get());
        } else {  // Function
          auto fn = util::as<code::Function*>(owner);
          auto fn_type = fn->type.as<code::TypeFunction>();
          fn_type->_dependencies.insert(mod.get());
        }
      }

      return;
    }
  }
}

}  // namespace pass
}  // namespace arrow
