// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/resolver.hpp"
#include "arrow/generator.hpp"
#include "arrow/builder.hpp"
#include "arrow/log.hpp"

using arrow::Resolver;
using arrow::resolve;

void Resolver::visit_member(ast::Member& x) {
  // TODO: Implement


  // // Resolve the base operand
  // auto op_type = build_scalar(*x.operand);
  //
  // // Decide how to handle this
  // // This could be <module>.<id> which is a very simple pass-through
  // // Or it could be <expr>.<id> which gets more fun
  // if (op_type->is<code::Module>()) {
  //   auto& mod = op->as<code::Module>();
  //
  //   // Delegate and resolve the type in the module
  //   auto type = resolve(_g, &mod.scope, *x.id);
  //
  //   // Push the new type back into the stack
  //   _stack.push(type);
  // } else if (op_type->is<code::Value>()) {
  //   // Determine and continue based on the type of the value
  //   auto& val = op->as<code::Value>();
  //   auto type = val.type();
  //   if (type->is<code::StructureType>()) {
  //     auto& st = type->as<code::StructureType>();
  //
  //     // Check for the member
  //     std::shared_ptr<ast::StructureMember> member = nullptr;
  //     auto& ctx = st.context->as<ast::Structure>();
  //     unsigned idx = 0;
  //     bool found = false;
  //     for ( ; idx < ctx.members.size(); idx++) {
  //       member = ctx.members.at(idx);
  //       if (member->name->text == x.id->text) {
  //         found = true;
  //         break;
  //       }
  //     }
  //
  //     if (!found) {
  //       Log::get().error("no member named '%s' in '%s'",
  //         x.id->text.c_str(),
  //         type->name().c_str());
  //
  //       return;
  //     }
  //
  //     // Push the type of this member
  //     _stack.push(st.member_type(_g, idx));
  //   } else {
  //     // FIXME: Check, handle, and error-out
  //   }
  // } else {
  //   // FIXME: Check, handle, and error-out
  // }
}
