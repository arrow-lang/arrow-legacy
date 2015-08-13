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
  x.operand->accept(*this);
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

      Match(*member->second) {
        Case(code::Function& fn) {
          // Check if we've analyzed the function ..
          auto func_type = fn.type.as<code::TypeFunction>();
          if (func_type->_is_analyzed) return;

          // Analyze the function context ..
          AnalyzeUsage(_ctx, fn.scope->parent()).run(*fn.context);
        } break;
      } EndMatch;

      return;
    }
  }

  // Resolve the type of the operand
  auto type = Resolve(_scope).run(*x.operand);
  if (!type) return;

  Match(*type) {
    Case(code::TypeStructure& struct_) {
      auto& members = struct_.members;
      for (auto& mem : members) {
        if (mem->keyword == x.member) {
          // Found it!
          return;
        }
      }
    } break;
  } EndMatch;

  Log::get().error(
    x.span, "type '%s' has no member '%s'",
    type->name().c_str(),
    x.member.c_str());
}

}  // namespace pass
}  // namespace arrow
