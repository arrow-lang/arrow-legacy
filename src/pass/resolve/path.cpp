// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/util.hpp"
#include "arrow/pass/resolve.hpp"
#include "arrow/match.hpp"

namespace arrow {
namespace pass {

void Resolve::visit_path(ast::Path& x) {
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

      auto type = util::type_of(member->second);
      if (type) {
        _stack.push_front(type);
      }

      return;
    }
  }

  // Resolve the type of the operand
  auto type = Resolve(_scope).run(*x.operand);
  if (!type) return;

  Match(*type) {
    Case(code::TypeStructure& struct_) {
      auto& members = struct_.members;
      // FIXME: Map
      for (auto& mem : members) {
        if (mem->keyword == x.member) {
          // Found it!
          _stack.push_front(mem->type);

          return;
        }
      }
    } break;
  } EndMatch;
}

}  // namespace pass
}  // namespace arrow
