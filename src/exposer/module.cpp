// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/exposer.hpp"
#include "arrow/resolver.hpp"
#include "arrow/generator.hpp"
#include "arrow/log.hpp"

using arrow::resolve;
using arrow::Exposer;
namespace code = arrow::code;
namespace ast = arrow::ast;

void Exposer::visit_module(ast::Module& x) {
  // Create (and set) the module item
  auto mod = std::make_shared<code::Module>(&x, x.name, &_scope);
  _scope.set(x.name, mod);

  // Create a module function
  mod->function = LLVMAddFunction(
    _g._mod, (x.name + "@init").c_str(),
    LLVMFunctionType(LLVMVoidType(), nullptr, 0, 0));

  LLVMAppendBasicBlock(mod->function, "");

  // Push us in the list of modules (to later build)
  _g._modules.push_back(mod);

  // Extract named items from the sequence (for name hoisting)
  arrow::Exposer nested_exposer{_g, mod->scope};
  for (auto& item : x.sequence) {
    item->accept(nested_exposer);
  }
}
