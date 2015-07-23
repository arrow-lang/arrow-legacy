// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/analyze-type.hpp"

namespace arrow {
namespace pass {

static Ref<code::Type> promote(Ref<code::Type> type) {
  if (type.is<code::TypeSizedInteger>()) {
    return new code::TypeInteger();
  } else if (type.is<code::TypeTuple>()) {
    // Look for and promote sized-integers to integers
    // Create the initial type
    auto tuple = type.as<code::TypeTuple>();
    Ref<code::TypeTuple> result = new code::TypeTuple();

    // Iterate through the elements; and type each
    result->elements.reserve(tuple->elements.size());
    for (auto& element : tuple->elements) {
      result->elements.push_back(promote(element));
    }

    return result;
  }

  return type;
}

void AnalyzeType::run(ast::Node& x) {
  do {
    // Clear slate
    _incomplete = false;
    _assign.clear();

    // Run the base visitor
    ast::Visitor::run(x);

    // Enumerate through each decl. and ensure that all types have been
    // annotated through context
    for (auto& ref : _assign) {
      auto item = _scope->find(ref.first);

      // Skip this if its an external slot (for now)
      // TODO: Make these part of the process
      if (item.is<code::ExternSlot>()) continue;

      auto slot = item.as<code::Slot>();
      auto& assign_set = ref.second;

      // FIXME: Reduce the COMMON type among all types

      // If the type of this assignment is invalid (undefined);
      // we're not done yet ..
      if (assign_set.size() == 0) {
        // No assignments .. type is None
        slot->type = new code::TypeNone();
      } else if (!assign_set[0].type || assign_set[0].type->is_unknown()) {
        _incomplete = true;
        continue;
      } else {
        // Mark the type
        slot->type = assign_set[0].type;

        // Ensure that integral slots always end up as `int` without
        // an explicit type annotation
        slot->type = promote(slot->type);
      }
    }

    if (Log::get().count("error") > 0) break;
  } while (_incomplete);
}

}  // namespace pass
}  // namespace arrow
