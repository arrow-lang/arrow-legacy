// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/builder.hpp"
#include "arrow/generator.hpp"
#include "arrow/extractor.hpp"
#include "arrow/log.hpp"
#include "arrow/extractor.hpp"

using arrow::Builder;
namespace code = arrow::code;
namespace ast = arrow::ast;

void Builder::visit(ast::Module& node) {
  // Create the module item
  // FIXME: The parser doesn't give the module a name (yet)
  auto mod = std::make_shared<code::Module>("_", &_scope);

  // Extract named items from the sequence (for name hoisting)
  arrow::Extractor extractor{_g, mod->scope};
  node.accept(extractor);

  // Take the remaining non-hoisted items and continue normal
  // iterative building
  // auto& rem = extractor.remaining();
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
