// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/type.hpp"

namespace arrow {
namespace pass {

void Type::visit_type_function(ast::TypeFunction& x) {
  // Get the type of the parameter
  auto result_type = Type(_scope).run(*x.result);
  if (!result_type) return;

  // Create the initial type IR
  Ref<code::TypeFunction> fn = new code::TypeFunction(
    code::TypeFunction::Abi::Native,
    result_type
  );

  // Iterate through the elements; and type each
  for (auto& param : x.parameters) {
    auto param_type = Type(_scope).run(*param);
    if (!param_type) return;

    fn->parameters.push_back(param_type);
  }

  _stack.push_front(fn);
}

void Type::visit_type_parameter(ast::TypeParameter& x) {
  // Get the type of the parameter
  auto type = Type(_scope).run(*x.type);
  if (!type) return;

  _stack.push_front(new code::TypeParameter(x.keyword, type, x.is_mutable));
}

}  // namespace pass
}  // namespace arrow
