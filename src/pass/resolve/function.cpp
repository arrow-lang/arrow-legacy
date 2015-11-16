// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/resolve.hpp"
#include "arrow/pass/type.hpp"
#include "arrow/match.hpp"

namespace arrow {
namespace pass {

// FIXME: Looks a lot like Resolve::visit_extern_function
void Resolve::visit_function(ast::Function& x) {
  // Get the result type
  auto result = Type(_scope).run(*x.result_type);
  if (!result) return;

  // Declare the initial function type
  Ref<code::TypeFunction> type = new code::TypeFunction(
    code::TypeFunction::Abi::Native, result);

  // Iterate over parameters and resolve each type
  type->parameters.reserve(x.parameters.size());
  for (auto& param : x.parameters) {
    auto ptype = Resolve(_scope).run(*param);
    if (!ptype) return;

    type->parameters.push_back(ptype);
  }

  _stack.push_front(type);
}

// FIXME: Looks a lot like Resolve::visit_function
void Resolve::visit_extern_function(ast::ExternFunction& x) {
  // Get the result type
  auto result = Type(_scope).run(*x.result_type);
  if (!result) return;

  // Declare the initial function type
  Ref<code::TypeFunction> type = new code::TypeFunction(
    code::TypeFunction::Abi::C, result);

  // Iterate over parameters and resolve each type
  type->parameters.reserve(x.parameters.size());
  for (auto& param : x.parameters) {
    auto ptype = Resolve(_scope).run(*param);
    if (!ptype) return;

    type->parameters.push_back(ptype);
  }

  _stack.push_front(type);
}

void Resolve::visit_parameter(ast::Parameter& x) {
  // Type the parameter type
  auto type = Type(_scope).run(*x.type);
  if (!type) return;

  // Infer the name from the parameter pattern
  // A parameter only has a name if its an identifier pattern
  std::string name;
  bool is_mutable = false;
  Match(*x.pattern) {
    Case(const ast::PatternIdentifier& id) {
      name = id.text;
      is_mutable = id.is_mutable;
    } break;

    Otherwise() {
      // Name is "" to indicate unnamed
      name = "";
    } break;
  } EndMatch;

  // Create and push the type
  _stack.push_front(new code::TypeParameter(name, type, is_mutable));
}

}  // namespace pass
}  // namespace arrow
