// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/pass/build.hpp"

namespace arrow {

static bool _expand_pattern(
  ast::Pattern& pattern,
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
      item->handle = LLVMAddGlobal(
        ctx.mod, item->type->handle(), item->name.c_str());
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
  // Expand the pattern ..
  _expand_pattern(*x.pattern, _scope, _ctx);
}

}  // namespace pass
}  // namespace arrow
