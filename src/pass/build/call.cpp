// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/util.hpp"
#include "arrow/pass/build.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

static Ref<code::TypeParameter> _param_for(
  Ref<code::TypeFunction> type, ast::Argument& arg, unsigned arg_index
) {
  Ref<code::TypeParameter> param = nullptr;
  if (arg.name.size() > 0) {
    param = type->find_parameter(arg.name);
    if (!param) {
      Log::get().error(
        arg.span,
        "'%s' is an invalid keyword argument for this function",
        arg.name.c_str());

      return nullptr;
    }
  } else {
    param = type->parameters.at(arg_index);
  }

  return param;
}

void Build::visit_call(ast::Call& x) {
  // Determine if we are a constructor (struct type) or a function
  if (x.operand.is<ast::Identifier>() ||
      x.operand.is<ast::Path>()) {
    auto item = util::get_item(_scope, *x.operand);
    if (item && item.is<code::Structure>()) {
      return do_struct_ctor(x, item);
    }
  }

  // Build the functor operand ..
  auto op = Build(_ctx, _scope).run_scalar(*x.operand);
  if (!op) return;

  // Grab the type of the function ..
  // TODO(mehcode): Errors when this isn't a function type
  auto type = op->type.as<code::TypeFunction>();

  // Resolve and build each argument (if any)
  // TODO(mehcode): Check for mis-matched types
  std::vector<LLVMValueRef> arguments(type->parameters.size());
  unsigned arg_index = 0;
  for (auto& arg_node : x.arguments) {
    // Build the argument expression
    auto arg = Build(_ctx, _scope).run_scalar(*arg_node->expression);
    if (!arg) return;

    LLVMValueRef arg_value = nullptr;

    Ref<code::TypeParameter> param = _param_for(type, *arg_node, arg_index);
    if (!param) return;

    if (param->is_mutable) {
      // Use the address
      arg_value = arg->get_address(_ctx);
    } else {
      // Cast the argument to the appropriate type
      arg = util::cast(_ctx, arg, *arg_node->expression, param->type, false);
      if (!arg) return;

      arg_value = arg->get_value(_ctx);
    }

    if (arg_node->name.size() > 0) {
      auto param_index = 0;
      auto match = false;
      for (auto& param : type->parameters) {
        if (param->keyword == arg_node->name) {
          match = true;
          break;
        }

        param_index += 1;
      }

      if (match) {
        if (arguments[param_index] != nullptr) {
          Log::get().error(
            x.span,
            "got multiple values for the argument: '%s'",
            arg_node->name.c_str());

          return;
        }

        // Add it to the keyword arguments
        arguments[param_index] = arg_value;
      } else {
        return;
      }
    } else {
      // Add it to the positional arguments
      arguments[arg_index] = arg_value;
      arg_index += 1;
    }
  }

  // Check for missing parameters
  unsigned missing_cnt = 0;
  for (unsigned idx = 0; idx < arguments.size(); ++idx) {
    if (arguments[idx] == nullptr) {
      missing_cnt += 1;
    }
  }

  if (missing_cnt > 0) {
    Log::get().error(x.span, "expected %d arguments but received %d",
      type->parameters.size(), type->parameters.size() - missing_cnt);

    return;
  }

  // Build the call IR
  auto res = LLVMBuildCall(
    _ctx.irb, op->get_value(_ctx), arguments.data(), arguments.size(), "");

  // TODO: Add sext/zext attributes if this function is of the C ABI

  // Push the resultant value
  _stack.push_front(new code::Value(res, type->result));
}

void Build::do_struct_ctor(ast::Call& x, Ref<code::Structure> item) {
  auto& type = item->type;

  // Segregate const and non-const elements
  // LLVM can create the initial tuple of const elements but subsequent
  // `insertvalue` instructions must be done to add non-constant values.
  std::vector<LLVMValueRef> const_values(type->members.size());
  std::vector<std::pair<unsigned, LLVMValueRef>> non_const_values;

  unsigned idx = 0;
  for (auto& arg_node : x.arguments) {
    // Build the argument expression
    auto arg = Build(_ctx, _scope).run_scalar(*arg_node->expression);
    if (!arg) return;

    Ref<code::TypeMember> mem = nullptr;
    if (arg_node->name.size() > 0) {
      mem = type->find_member(arg_node->name);
      if (!mem) {
        Log::get().error(
          arg_node->span,
          "type '%s' has no member '%s'",
          type->name().c_str(),
          arg_node->name.c_str());

        return;
      }
    } else {
      mem = type->members.at(idx);
    }

    // Cast the argument to the appropriate type
    arg = util::cast(_ctx, arg, *arg_node->expression, mem->type, false);
    if (!arg) return;

    auto mem_index = idx;
    if (arg_node->name.size() > 0) {
      auto match = false;
      auto idx = 0;
      for (auto& mem : type->members) {
        if (mem->keyword == arg_node->name) {
          match = true;
          break;
        }

        idx += 1;
      }

      if (match) {
        if (const_values[idx] != nullptr) {
          Log::get().error(
            x.span,
            "got multiple values for the member: '%s'",
            arg_node->name.c_str());

          return;
        }

        // Add it to the keyword arguments
        mem_index = idx;
      } else {
        return;
      }
    }

    auto value = arg->get_value(_ctx);
    if (LLVMIsConstant(value)) {
      const_values[mem_index] = value;
    } else {
      const_values[mem_index] = LLVMGetUndef(LLVMTypeOf(value));
      non_const_values.push_back({mem_index, value});
    }

    idx += 1;
  }

  // Check for missing args
  unsigned missing_cnt = 0;
  for (unsigned idx = 0; idx < const_values.size(); ++idx) {
    if (const_values[idx] == nullptr) {
      missing_cnt += 1;
    }
  }

  if (missing_cnt > 0) {
    Log::get().error(x.span, "expected %d members but received %d",
      type->members.size(), type->members.size() - missing_cnt);

    return;
  }

  // Create the initial const struct
  auto handle =  LLVMConstNamedStruct(
    type->handle(), const_values.data(), const_values.size());

  // Iterate through each non-const value
  for (auto& ncv : non_const_values) {
    handle = LLVMBuildInsertValue(
      _ctx.irb, handle, ncv.second, ncv.first, "");
  }

  // Push the resultant value
  _stack.push_front(new code::Value(handle, type));
}

}  // namespace pass
}  // namespace arrow
