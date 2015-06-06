// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/builder.hpp"
#include "arrow/generator.hpp"
#include "arrow/expose.hpp"
#include "arrow/log.hpp"

using arrow::Builder;
namespace code = arrow::code;
namespace ast = arrow::ast;

void Builder::visit_module(ast::Module& node) {
  // Create (and set) the module item
  auto mod = std::make_shared<code::Module>(node.name, &_scope);
  _cs->set(node.name, mod);

  // Extract named items from the sequence (for name hoisting)
  arrow::Expose{_g, mod->scope}.run(node);

  // Take the remaining non-hoisted items and continue normal
  // iterative building
  for (auto& item : node.sequence) {
    // Remember the size of the stack at this point (so we can
    // detect if an item gets pushed; and then remove it)
    auto cnt = _stack.size();

    build(*item, &(mod->scope));

    // Remove anything pushed onto the stack
    for (unsigned i = 0; i < (_stack.size() - cnt); ++i) {
      _stack.pop();
    }
  }
}
