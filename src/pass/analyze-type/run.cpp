// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/analyze-type.hpp"

namespace arrow {
namespace pass {

void AnalyzeType::run(ast::Node& x) {
  // Maximum # of times we can run and do nothing
  int max_null_run = 5;
  do {
    // Clear slate
    _incomplete = false;
    _assign.clear();
    _modules.clear();
    unsigned cnt = 0;
    unsigned req = 0;

    // Run the base visitor
    ast::Visitor::run(x);

    // Enumerate through each decl. and ensure that all types have been
    // annotated through context
    for (auto& ref : _assign) {
      auto slot = ref.first;

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
          type = code::intersect_all(type_set);
          if (!type) {
            Log::get().error(slot->context->span,
              "unable to infer a type for variable %s",
              slot->name.c_str());
          }
        }

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
