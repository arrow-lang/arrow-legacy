// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <cassert>
#include "arrow/match.hpp"
#include "arrow/util.hpp"
#include "arrow/pass/build.hpp"
#include "arrow/pass/declare.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

bool Declare::_expand_pattern(ast::Pattern& pattern) {
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

      if (!local) {
        item->set_address(LLVMAddGlobal(
          _ctx.mod, item->type->handle(), item->name.c_str()));

        // TODO(mehcode): If we're building a library and this is exported..
        LLVMSetLinkage(item->get_address(_ctx), LLVMInternalLinkage);

        // For a non-constant initializer, we have to set an initial /zero/
        // initializer and add a store to the module init function
        LLVMSetInitializer(
          item->get_address(_ctx), LLVMConstNull(item->type->handle()));
      }
    } break;

    Case(ast::PatternTuple& x) {
      unsigned idx = 0;
      for (auto& element : x.elements) {
        if (!_expand_pattern(*element)) {
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

void Declare::visit_slot(ast::Slot& x) {
  // Expand the pattern ..
  _expand_pattern(*x.pattern);
}

}  // namespace pass
}  // namespace arrow
