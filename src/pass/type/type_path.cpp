// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/type.hpp"
#include "arrow/match.hpp"
#include "arrow/util.hpp"

namespace arrow {
namespace pass {

void Type::visit_type_path(ast::TypePath& x) {
  // Get the initial item
  auto item = _scope->find(x.segments[0]);
  if (!item) {
    Log::get().error(
      x.span, "use of unresolved name '%s'", x.segments[0].c_str());

    return;
  }

  // Take successive items as members from the first
  for (unsigned index = 1; index < x.segments.size(); ++index) {
    Match(*item) {
      Case(code::Import& imp) {
        auto ref = imp.module->items.find(x.segments[index]);
        if (ref != imp.module->items.end()) {
          item = ref->second;
          continue;
        }
      } break;
    } EndMatch;

    // Couldn't figure it out ..
    auto type = util::type_of(item);
    if (type) {
      Log::get().error(
        x.span, "type '%s' has no member '%s'",
        type->name().c_str(),
        x.segments[index].c_str());

      return;
    }
  }

  // Pull the referred type out of the typename (and push to the stack)
  if (item.is<code::Typename>()) {
    _stack.push_front(item.as<code::Typename>()->type);
  } else if (item.is<code::Structure>()) {
    _stack.push_front(item.as<code::Structure>()->type);
  }
}

}  // namespace pass
}  // namespace arrow
