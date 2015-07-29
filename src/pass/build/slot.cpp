// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <cassert>
#include "arrow/match.hpp"
#include "arrow/pass/build.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {

static bool _expand_pattern(
  ast::Pattern& pattern,
  Ref<code::Value> initializer,
  Ref<code::Scope> scope,
  Compiler::Context& ctx
) {
  Match(pattern) {
    Case(ast::PatternWildcard& x) {
      XTL_UNUSED(x);

      // Do nothing
    } break;

    Case(ast::PatternIdentifier& x) {
      XTL_UNUSED(x);

      // Pull out the previously-exposed item
      auto item = scope->find(&pattern).as<code::Slot>();
      if (!item || !item->type) return false;
      if (item->type.is<code::TypeNone>()) return true;

      // Determine if this is a global or local slot
      bool local = false;
      if (scope->get_owner()) {
        local = typeid(*scope->get_owner()) == typeid(code::Function);
      }

      if (local) {
        item->set_address(LLVMBuildAlloca(
          ctx.irb, item->type->handle(), item->name.c_str()));
      } else {
        item->set_address(LLVMAddGlobal(
          ctx.mod, item->type->handle(), item->name.c_str()));

        // TODO(mehcode): If we're building a library and this is exported..
        LLVMSetLinkage(item->get_address(ctx), LLVMInternalLinkage);
      }

      // If we have an initializer ..
      auto ptr = item->get_address(ctx);
      if (initializer) {
        // Get the value of the initializer
        auto handle = initializer->get_value(ctx);
        if (!local && LLVMIsConstant(handle)) {
          LLVMSetInitializer(ptr, handle);

          // If we have an initializer and are not mutable;
          // this is a constant
          if (!local && !x.is_mutable) {
            LLVMSetGlobalConstant(ptr, true);
          }
        } else {
          // For a non-constant initializer, we have to set an initial /zero/
          // initializer and add a store to the module init function
          if (!local) {
            LLVMSetInitializer(ptr, LLVMConstNull(item->type->handle()));
          }

          LLVMBuildStore(ctx.irb, handle, ptr);
        }
      } else if (!local) {
        // Without an initializer; need to set to be zero initialized
        LLVMSetInitializer(ptr, LLVMConstNull(item->type->handle()));
      }
    } break;

    Case(ast::PatternTuple& x) {
      unsigned idx = 0;
      for (auto& element : x.elements) {
        auto val = initializer->at(ctx, idx);
        if (!val) return false;

        if (!_expand_pattern(*element, val, scope, ctx)) {
          return false;
        }

        idx += 1;
      }
    } break;

    Otherwise() {
      // Unreachable
      return false;
    }
  } EndMatch;

  return true;
}

namespace pass {

void Build::visit_slot(ast::Slot& x) {
  // Check for an initializer ..
  Ref<code::Value> initializer = nullptr;
  if (x.initializer) {
    initializer = Build(_ctx, _scope).run_scalar(*x.initializer);
    if (!initializer) return;

    // Resolve the type of the `slot`
    auto slot_type = Resolve(_scope).run(x);
    if (!slot_type) return;

    // Cast the initializer to the `slot` type
    initializer = do_cast(initializer, *x.initializer, slot_type, false);
    if (!initializer) return;
  }

  // Expand the pattern ..
  _expand_pattern(*x.pattern, initializer, _scope, _ctx);
}

}  // namespace pass
}  // namespace arrow
