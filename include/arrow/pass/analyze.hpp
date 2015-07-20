// // Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.
//
// // Distributed under the MIT License
// // See accompanying file LICENSE
//
// #ifndef ARROW_PASS_ANALYZE_H
// #define ARROW_PASS_ANALYZE_H 1
//
// #include "arrow/ast.hpp"
// #include "arrow/compiler.hpp"
// #include "arrow/code.hpp"
//
// namespace arrow {
// namespace pass {
//
// class Analyze : public ast::Visitor {
//  public:
//   explicit Analyze(Ref<code::Scope> scope) : _scope(scope), _incomplete(false) {
//   }
//
//   virtual ~Analyze() noexcept;
//
//   void run(ast::Node& x);
//
//   // Slot
//   virtual void visit_slot(ast::Slot& x);
//   virtual void visit_extern_slot(ast::ExternSlot& x);
//
//   // Function
//   virtual void visit_extern_function(ast::ExternFunction& x);
//
//   // Use [!]
//   virtual void visit_id(ast::Identifier& x);
//
//   // Assign [!]
//   virtual void visit_assign(ast::Assign& x);
//
//  private:
//   bool is_coercible_to(ast::Node& from, ast::Node& to);
//   bool is_coercible_to(ast::Node& from, ast::Type& to);
//   bool is_coercible_to(Ref<code::Type> from, Ref<code::Type> to);
//
//   bool require_is_coercible_to(ast::Node& from, ast::Node& to);
//   bool require_is_coercible_to(ast::Node& from, ast::Type& to);
//
//   bool _expand_pattern(
//     ast::Pattern& pattern,
//     Ref<code::Type> type_annotation,
//     Ref<code::Type> type_initializer);
//
//   bool _expand_assign(ast::Assign& node, ast::Node& lhs, Ref<code::Type> type);
//
//   // The scope to emplace the exposed items into.
//   Ref<code::Scope> _scope;
//
//   // struct Declaration {
//   //   /// Type of the declaration (can be null if there was no annotation).
//   //   Ref<code::Type> type;
//   //
//   //   /// Name of the declaration.
//   //   std::string name;
//   //
//   //   /// If the declaration refers to a mutable slot.
//   //   bool is_mutable;
//   //
//   //   /// If the declaration is external.
//   //   bool is_external;
//   // };
//   //
//   // bool _incomplete;
//   // std::unordered_map<std::string, ast::Node*> _x_name;
//   // std::unordered_map<ast::Node*, Declaration> _x_declare;
//   // std::unordered_map<ast::Node*, std::vector<Assignment>> _x_assign;
// };
//
// }  // namespace pass
// }  // namespace arrow
//
// #endif  // ARROW_PASS_ANALYZE_H
