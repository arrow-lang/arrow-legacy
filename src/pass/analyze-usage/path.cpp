// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/pass/analyze-usage.hpp"
#include "arrow/pass/resolve.hpp"
#include "arrow/util.hpp"

namespace arrow {
namespace pass {

void AnalyzeUsage::visit_path(ast::Path& x) {
  // Run the base method (analyze the operand and each argument)
  Visitor::visit_path(x);
  if (Log::get().count("error") > 0) return;

  // Check if we're running the `.` operator on an identifier [..]
  if (x.operand.is<ast::Identifier>()) {
    // Lookup this identifier and check if its a module [..]
    auto text = x.operand.as<ast::Identifier>()->text;
    auto item = _scope->find(text);
    if (item.is<code::Import>()) {
      auto imp = item.as<code::Import>();
      auto mod = imp->module;
      auto member = mod->items.find(x.member);
      if (member == mod->items.end()) {
        Log::get().error(
          x.span, "module '%s' has no item '%s'",
          mod->name.c_str(),
          x.member.c_str());

        return;
      }

      // bool is_static_use = false;
      // Match(*member->second) {
      //   Case(code::Slot& slot) {
      //     is_static_use = slot.is_static;
      //   } break;
      //
      //   Case(code::ExternSlot& slot) {
      //     XTL_UNUSED(slot);
      //     is_static_use = true;
      //   } break;
      //
      //   Case(code::ExternFunction& fn) {
      //     XTL_UNUSED(fn);
      //     is_static_use = true;
      //   } break;
      //
      //   Case(code::Function& fn) {
      //     auto type = fn.type.as<code::TypeFunction>();
      //
      //     is_static_use = true;
      //     for (auto& slot : type->_use) {
      //       if (!slot->is_static) {
      //         is_static_use = false;
      //         break;
      //       }
      //     }
      //
      //     for (auto& slot : type->_assign) {
      //       if (!slot.first->is_static) {
      //         is_static_use = false;
      //         break;
      //       }
      //     }
      //   } break;
      // } EndMatch;
      //
      // auto cur = util::current_module(_scope);
      // if (!is_static_use) {
      //   // Module used a non-static from another module
      //   // Record a dependency
      //   cur->dependencies.insert(mod);
      // }

      return;
    }
  }

  // Resolve the type of the operand
  auto type = Resolve(_scope).run(*x.operand);
  if (!type) return;

  // FIXME: Not implemented!
  Log::get().error(
    x.span, "type '%s' has no member '%s'",
    type->name().c_str(),
    x.member.c_str());
}

}  // namespace pass
}  // namespace arrow
