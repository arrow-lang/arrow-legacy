// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/pass/build.hpp"

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
      // TODO(mehcode): `scope->get<T>`
      auto item = scope->get(&pattern).as<code::Slot>();
      if (!item || !item->type) return false;

      // TODO(mehcode): If we are inside a function this is a local, else global
      // NOTE: At the moment; all slots are assumed globals
      item->set_address(LLVMAddGlobal(
        ctx.mod, item->type->handle(), item->name.c_str()));

      // If we have an initializer ..
      // TODO(mehcode): cast
      auto ptr = item->get_address(ctx);
      if (initializer) {
        auto handle = initializer->get_value(ctx);
        if (LLVMIsConstant(handle)) {
          LLVMSetInitializer(ptr, handle);

          // If we have an initializer and are not mutable;
          // this is a constant
          if (!x.is_mutable) {
            LLVMSetGlobalConstant(ptr, true);
          }
        } else {
          // For a non-constant initializer, we have to set an initial /zero/
          // initializer and add a store to the module init function
          LLVMSetInitializer(ptr, LLVMConstNull(item->type->handle()));
          LLVMBuildStore(ctx.irb, handle, ptr);
        }
      } else {
        // Without an initializer; need to set to be zero initialized
        LLVMSetInitializer(ptr, LLVMConstNull(item->type->handle()));
      }
    } break;

    Case(ast::PatternTuple& x) {
      LLVMValueRef ptr = nullptr;
      if (initializer->has_address()) {
        ptr = initializer->get_address(ctx);
      } else {
        // Create an `unnamed_addr` constant of the initializer
        // NOTE: This is necessary because we cannot do "." direclty on
        //       a constant value (tuple); this all gets optimized away.
        auto iv = initializer->get_value(ctx);
        ptr = LLVMAddGlobal(
          ctx.mod, initializer->type->handle(), "~temp");
        LLVMSetGlobalConstant(ptr, true);
        LLVMSetUnnamedAddr(ptr, true);
        LLVMSetLinkage(ptr, LLVMInternalLinkage);
        if (LLVMIsConstant(iv)) {
          LLVMSetInitializer(ptr, iv);
        } else {
          LLVMSetInitializer(
            ptr, LLVMConstNull(initializer->type->handle()));

          LLVMBuildStore(ctx.irb, iv, ptr);
        }
      }

      auto type_tuple = initializer->type.as<code::TypeTuple>();
      unsigned idx = 0;
      for (auto& element : x.elements) {
        auto handle = LLVMBuildStructGEP(ctx.irb, ptr, idx, "");
        Ref<code::Value> val = new code::Value(
          handle, type_tuple->elements.at(idx));

        if (!_expand_pattern(*element, val, scope, ctx)) {
          return false;
        }

        idx += 1;
      }
    } break;

    Otherwise() {
      // FIXME(mehcode): Implement
      Log::get().error(
        pattern.span, "not (yet) implemented");

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
  }

  // Expand the pattern ..
  _expand_pattern(*x.pattern, initializer, _scope, _ctx);
}

}  // namespace pass
}  // namespace arrow
