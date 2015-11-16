// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/build.hpp"
#include "arrow/pass/resolve.hpp"
#include "arrow/match.hpp"

namespace arrow {
namespace pass {

void Build::visit_path(ast::Path& x) {
  // Check if we're running the `.` operator on an identifier [..]
  if (x.operand.is<ast::Identifier>()) {
    // Lookup this identifier and check if its a module [..]
    auto text = x.operand.as<ast::Identifier>()->text;
    auto item = _scope->find(text);
    if (item.is<code::Import>()) {
      auto imp = item.as<code::Import>();
      auto mod = imp->module;
      auto member = mod->items.find(x.member);

      _stack.push_front(member->second);
      return;
    }
  }

  // Build the type of the operand
  auto item = Build(_ctx, _scope).run_scalar(*x.operand);
  if (!item) return;

  Match(*item->type) {
    Case(code::TypeStructure& struct_) {
      auto& members = struct_.members;
      // FIXME: Map
      unsigned idx = 0;
      for (auto& mem : members) {
        if (mem->keyword == x.member) {
          // Found it!
          auto handle = LLVMBuildStructGEP(
            _ctx.irb, item->get_address(_ctx), idx, "");

          _stack.push_front(new code::Value(handle, mem->type));

          return;
        }

        idx += 1;
      }
    } break;
  } EndMatch;
}

}  // namespace pass
}  // namespace arrow
