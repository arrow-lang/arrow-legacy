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
  // Maximum # of times we can run and do nothing
  int max_null_run = 5;
  do {
    // Clear slate
    _incomplete = false;
    _assign.clear();
    unsigned cnt = 0;
    unsigned req = 0;

    // Run the base visitor
    ast::Visitor::run(x);

    // Enumerate through each decl. and ensure that all types have been
    // annotated through context
    for (auto& ref : _assign) {
      auto item = _scope->find(ref.first);

      // Skip this if its an external slot (for now)
      if (item.is<code::ExternSlot>()) continue;

      auto slot = item.as<code::Slot>();

      // If this slot already has a type; get out
      if (slot->type && !slot->type->is_unknown()) continue;

      auto& assign_set = ref.second;
      req += 1;

      // If the type of this assignment is invalid (undefined);
      // we're not done yet ..
      if (assign_set.size() > 0) {
        std::vector<Ref<code::Type>> type_set;

        // Check if any of the types are unknown or invalid
        bool invalid = false;
        for (auto& assign : assign_set) {
          if (!assign.type || assign.type->is_unknown()) {
            invalid = true;
            break;
          }

          type_set.push_back(assign.type);
        }

        Ref<code::Type> type = nullptr;
        if (!invalid) {
          // Resolve the common type ..
          type = code::instersect_all(type_set);
          if (!type) {
            Log::get().error(item->context->span,
              "unable to infer a type for variable %s",
              item->name.c_str());
          }
        }

        // FIXME: Reduce the COMMON type among all types
        if (invalid) {
          if (max_null_run <= 0) {
            slot->type = new code::TypeNone();
          } else {
            _incomplete = true;
          }
        } else {
          // We did something
          cnt += 1;
          max_null_run = 5;

          // Mark the type
          slot->type = type;

          // Ensure that integral slots always end up as `int` without
          // an explicit type annotation
          slot->type = promote(slot->type);
        }
      } else if (max_null_run <= 0) {
        // We're done ..
        slot->type = new code::TypeNone();
      }
    }

    if (cnt == 0) {
      max_null_run -= 1;
    }

    if (req == 0 && max_null_run <= 0) {
      break;
    }

    if (Log::get().count("error") > 0) break;
  } while (_incomplete);
}

}  // namespace pass
}  // namespace arrow
