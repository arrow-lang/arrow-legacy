// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/pass/build.hpp"

namespace arrow {
namespace pass {

void Build::visit_function(ast::Function& x) {
  // Pull out the previously-exposed item
  // TODO(mehcode): `scope->find<T>`
  auto item = _scope->find(&x).as<code::Function>();
  if (!item) return;

  // Enter the function scope-block
  item->scope->enter(&x);

  auto type = item->type.as<code::TypeFunction>();
  auto handle = item->get_value(_ctx);

  // Add the top basic block
  auto last_block = LLVMGetInsertBlock(_ctx.irb);
  auto block = LLVMAppendBasicBlock(handle, "");
  LLVMPositionBuilderAtEnd(_ctx.irb, block);

  // Iterate and emplace parameters (in the function scope)
  for (unsigned param_idx = 0; param_idx < x.parameters.size(); ++param_idx) {
    auto& param = x.parameters.at(param_idx);

    // Allocate (and store) this parameter
    auto param_type = type->parameters.at(param_idx);
    auto param_handle = LLVMBuildAlloca(_ctx.irb, param_type->handle(), "");
    LLVMBuildStore(_ctx.irb, LLVMGetParam(handle, param_idx), param_handle);

    // Create a `code::Value` for the param
    Ref<code::Value> param_value = new code::Value(param_handle, param_type);

    // Expand the parameter
    if (!_expand_parameter_pattern(*param->pattern, param_value, item->scope)) {
      return;
    }
  }

  // Build the function body
  Build(_ctx, item->scope).run(*x.block);
  if (Log::get().count("error") > 0) return;

  // Has the function been terminated?
  if (!LLVMGetBasicBlockTerminator(LLVMGetLastBasicBlock(handle))) {
    // No; we need to terminate
    if (!type->result || type->result.is<code::TypeNone>()) {
      // No result type
      LLVMBuildRetVoid(_ctx.irb);
    // TODO(mehcode): Should really check if errors occured for this function
    } else if (Log::get().count("error") == 0) {
      // We should have gotten a result; report an error and exit
      Log::get().error(x.span, "not all code paths return a value");
      return;
    }
  }

  // Move instruction ptr back to where it was (if it was somewhere)
  if (last_block) {
    LLVMPositionBuilderAtEnd(_ctx.irb, last_block);
  }

  // Exit the function scope-block
  item->scope->exit();
}

bool Build::_expand_parameter_pattern(
  ast::Pattern& pattern, Ref<code::Value> value, Ref<code::Scope> scope
) {
  Match(pattern) {
    Case(ast::PatternWildcard& x) {
      XTL_UNUSED(x);

      // Do nothing
    } break;

    Case(ast::PatternIdentifier& x) {
      XTL_UNUSED(x);

      // Pull out the previously-exposed item
      auto item = scope->find(&pattern).as<code::Parameter>();
      if (!item || !item->type) return false;
      if (item->type.is<code::TypeNone>()) return true;

      // Set it's address
      item->set_address(value->get_address(_ctx));
    } break;

    Otherwise() {
      return false;
    }
  } EndMatch;

  return true;
}

}  // namespace pass
}  // namespace arrow
