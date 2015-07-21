// // Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.
//
// // Distributed under the MIT License
// // See accompanying file LICENSE
//
// #include "arrow/match.hpp"
// #include "arrow/pass/analyze.hpp"
// #include "arrow/pass/resolve.hpp"
// #include "arrow/pass/type.hpp"
//
// namespace arrow {
// namespace pass {
//
// bool Analyze::_expand_pattern(
//   ast::Pattern& pattern,
//   Ref<code::Type> type_annotation,
//   Ref<code::Type> type_initializer
// ) {
//   Match(pattern) {
//     Case(ast::PatternWildcard& x) {
//       XTL_UNUSED(x);
//
//       // Do nothing
//     } break;
//
//     Case(ast::PatternIdentifier& x) {
//       XTL_UNUSED(x);
//
//       // Pull out the previously-exposed item
//       auto item = _scope->find(&pattern).as<code::Slot>();
//       if (!item) return false;
//
//       // If we have an explicit type annotation ..
//       if (type_annotation) {
//         // .. mark this slot with the annotated type
//         item->type = type_annotation;
//       }
//
//       // Mark [declare]
//       _x_name[item->name] = &pattern;
//       _x_declare.insert({&pattern, {
//         type_annotation,
//         item->name,
//         item->is_mutable,
//         false
//       }});
//
//       // If we have an initializer ..
//       if (type_initializer) {
//         // Mark [assign]
//         // Emplace the assignment
//         _x_assign[&pattern] = {};
//         _x_assign[&pattern].push_back({
//           true,
//           0,
//           type_initializer ? type_initializer : nullptr
//         });
//       }
//     } break;
//
//     Case(ast::PatternTuple& x) {
//       if (!type_initializer || type_initializer->is_unknown()) {
//         // Initializer type is not ready (yet)
//         return false;
//       }
//
//       // Check to see if we have the exact # of elements
//       auto init_type_tuple = type_initializer.as<code::TypeTuple>();
//       if (x.elements.size() != init_type_tuple->elements.size()) {
//         Log::get().error(x.span,
//           "expected a tuple with %d elements, found one with %d elements",
//           init_type_tuple->elements.size(),
//           x.elements.size());
//
//         return false;
//       }
//
//       Ref<code::TypeTuple> annt_type_tuple = nullptr;
//       if (type_annotation) {
//         annt_type_tuple = type_annotation.as<code::TypeTuple>();
//       }
//
//       unsigned index = 0;
//       for (auto& element : x.elements) {
//         Ref<code::Type> el_type_annotation = nullptr;
//         Ref<code::Type> el_type_initializer = nullptr;
//
//         if (type_annotation) {
//           el_type_annotation = annt_type_tuple->elements.at(index);
//         }
//
//         el_type_initializer = init_type_tuple->elements.at(index);
//
//         if (!_expand_pattern(
//             *element, el_type_annotation, el_type_initializer)) {
//           return false;
//         }
//
//         index += 1;
//       }
//     } break;
//
//     Otherwise() {
//       return false;
//     }
//   } EndMatch;
//
//   return true;
// }
//
// void Analyze::visit_slot(ast::Slot& x) {
//   // Check for an explicit type annotation ..
//   Ref<code::Type> type_annotation = nullptr;
//   if (x.type) {
//     type_annotation = Type(_scope).run(*x.type);
//     if (!type_annotation) return;
//   }
//
//   // Check for an initializer ..
//   Ref<code::Type> type_initializer = nullptr;
//   if (x.initializer) {
//     // Analyze the initializer
//     x.initializer->accept(*this);
//     if (Log::get().count("error") > 0) return;
//
//     // Resolve the type of the initializer
//     type_initializer = Resolve(_scope).run(*x.initializer);
//     if (!type_initializer || type_initializer->is_unknown()) {
//       if (Log::get().count("error") == 0) {
//         _incomplete = true;
//         type_initializer = new code::TypeUnknown();
//       } else {
//         return;
//       }
//     }
//   }
//
//   if (type_initializer && type_annotation) {
//     // Check for mismatched types
//     if (!require_is_coercible_to(*x.initializer, *x.type)) {
//       if (Log::get().count("error") == 0) {
//         _incomplete = true;
//       }
//
//       return;
//     }
//   }
//
//   // Expand the pattern ..
//   _expand_pattern(
//     *x.pattern,
//     type_annotation,
//     type_initializer);
// }
//
// }  // namespace pass
// }  // namespace arrow
