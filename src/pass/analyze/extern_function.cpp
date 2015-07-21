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
// void Analyze::visit_extern_function(ast::ExternFunction& x) {
//   // Pull out the previously-exposed item
//   auto item = _scope->find(&x).as<code::ExternFunction>();
//   if (!item) return;
//
//   // Mark [declare]
//   _x_name[item->name] = &x;
//   _x_declare.insert({&x, {
//     item->type,
//     item->name,
//     false,
//     true
//   }});
// }
//
// }  // namespace pass
// }  // namespace arrow
