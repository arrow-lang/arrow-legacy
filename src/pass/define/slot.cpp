// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <cassert>
#include "arrow/util.hpp"
#include "arrow/pass/build.hpp"
#include "arrow/pass/define.hpp"
#include "arrow/pass/resolve.hpp"
#include "arrow/match.hpp"

namespace arrow {
namespace pass {

bool Define::_expand_pattern(
  ast::Pattern& pattern,
  Ref<code::Value> initializer
) {
  Match(pattern) {
    Case(ast::PatternWildcard& x) {
      XTL_UNUSED(x);

      // Do nothing
    } break;

    Case(ast::PatternIdentifier& x) {
      XTL_UNUSED(x);

      // Pull out the previously-exposed item
      auto item = _scope->find(&pattern).as<code::Slot>();
      if (!item || !item->type) return false;
      if (item->type.is<code::TypeNone>()) return true;

      // Determine if this is a global or local slot
      bool local = false;
      if (_scope->get_owner()) {
        local = typeid(*_scope->get_owner()) == typeid(code::Function);
      }

      // If we have a (static) initializer ..
      auto ptr = item->get_address(_ctx);
      if (initializer) {
        // Get the value of the initializer
        auto handle = initializer->get_value(_ctx);
        if (!local && LLVMIsConstant(handle)) {
          LLVMSetInitializer(ptr, handle);

          // If we have an initializer and are not mutable;
          // this is a constant
          if (!local && !x.is_mutable) {
            LLVMSetGlobalConstant(ptr, true);
          }
        }
      }
    } break;

    Case(ast::PatternTuple& x) {
      if (!initializer) break;

      unsigned idx = 0;
      for (auto& element : x.elements) {
        auto val = initializer->at(_ctx, idx);
        if (!val) return false;

        if (!_expand_pattern(*element, val)) {
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

void Define::visit_slot(ast::Slot& x) {
  // Check for a STATIC initializer ..
  Ref<code::Value> initializer = nullptr;
  if (x.initializer && util::is_static(*x.initializer)) {
    initializer = Build(_ctx, _scope).run_scalar(*x.initializer);
    if (!initializer) return;

    // Resolve the type of the `slot`
    auto slot_type = Resolve(_scope).run(x);
    if (!slot_type) return;

    // Cast the initializer to the `slot` type
    initializer = util::cast(
      _ctx, initializer, *x.initializer, slot_type, false);

    if (!initializer) return;
  }

  // Expand the pattern ..
  _expand_pattern(*x.pattern, initializer);
}

}  // namespace pass
}  // namespace arrow
