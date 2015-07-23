// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/expose.hpp"
#include "arrow/pass/type.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void Expose::visit_function(ast::Function& x) {
  // Add this (undefined and un-analyzed) to the current scope
  _scope->insert(new code::Function(
    /*context=*/&x,
    /*name=*/x.name,
    /*parent_scope=*/_scope));
}

}  // namespace pass
}  // namespace arrow
