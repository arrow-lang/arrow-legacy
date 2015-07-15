// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/pass/build.hpp"

namespace arrow {
namespace pass {

static bool do_assign(
  Compiler::Context& ctx, Ref<code::Value> lhs, Ref<code::Value> rhs
) {
  Match(*lhs) {
    Case(code::ValueTuple& x) {
      // Enumerate the elements
      unsigned idx = 0;
      for (auto& el : x.elements) {
        auto val = rhs->at(ctx, idx);
        if (!val) return false;
        if (!do_assign(ctx, el, val)) return false;
        idx += 1;
      }
    } break;

    Case(code::Value& x) {
      // Build a `Store`
      LLVMBuildStore(ctx.irb, rhs->get_value(ctx), x.get_address(ctx));
    } break;

    Otherwise() {
      return false;
    }
  } EndMatch;

  return true;
}

void Build::visit_assign(ast::Assign& x) {
  // Build each operand ..
  auto lhs = Build(_ctx, _scope).run_scalar(*x.lhs);
  auto rhs = Build(_ctx, _scope).run_scalar(*x.rhs);
  if (!lhs || !rhs) return;

  // Check if we are "assignable" ..
  if (!lhs->is_assignable()) {
    Log::get().error(x.span, "illegal left-hand side expression");
    return;
  }

  // Check for mismatched types ..
  // TODO(mehcode): This may make more sense to do in `Analyze`
  // TODO(mehcode): Should be a util for this regardless
  if (!lhs->type->equals(*rhs->type)) {
    Log::get().error(x.rhs->span,
      "mismatched types: expected `%s`, found `%s`",
      lhs->type->name().c_str(),
      rhs->type->name().c_str());

    return;
  }

  // Perform the assignment ..
  if (!do_assign(_ctx, lhs, rhs)) return;

  // Push RHS onto the stack
  _stack.push_front(rhs);
}

}  // namespace pass
}  // namespace arrow
