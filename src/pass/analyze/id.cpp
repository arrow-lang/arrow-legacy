// // Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.
//
// // Distributed under the MIT License
// // See accompanying file LICENSE
//
// #include "arrow/pass/analyze.hpp"
//
// namespace arrow {
// namespace pass {
//
// void Analyze::visit_id(ast::Identifier& x) {
//   // Check for a declared name
//   auto ref = _x_name.find(x.text);
//   if (ref == _x_name.end()) {
//     Log::get().error(
//       x.span, "use of unresolved name: '%s'", x.text.c_str());
//
//     return;
//   }
//
//   // If this is externally defined; get out
//   if (_x_declare[ref->second].is_external) return;
//
//   // Check for at least one definite assignment
//   bool is_assigned = false;
//   if (_x_assign.find(ref->second) != _x_assign.end()) {
//     auto& assign_set = _x_assign[ref->second];
//     for (auto& assignment : assign_set) {
//       if (assignment.is_definite) {
//         is_assigned = true;
//         break;
//       }
//     }
//   }
//
//   if (!is_assigned) {
//     Log::get().error(
//       x.span, "use of uninitialized variable: '%s'", x.text.c_str());
//
//     return;
//   }
// }
//
// }  // namespace pass
// }  // namespace arrow
