// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/match.hpp"
#include "arrow/util.hpp"

namespace arrow {
namespace util {

code::Module* current_module(Ref<code::Scope> scope) {
  auto owner = scope->get_owner();
  if (is<code::Module>(*owner)) {
    return as<code::Module*>(owner);
  }

  auto parent_scope = scope->parent();
  code::Module* result = nullptr;
  if (parent_scope) {
    result = current_module(parent_scope);
  }

  return result;
}

}  // namespace util
}  // namespace arrow
