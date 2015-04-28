// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/extractor.hpp"
#include "arrow/generator.hpp"
#include "arrow/log.hpp"

using arrow::Extractor;
namespace code = arrow::code;
namespace ast = arrow::ast;

#define IMPL_REM(N) \
  void Extractor::visit(ast::N& node) { _items.push_back(&node); }

Extractor::Extractor(arrow::Generator& g, code::Scope& scope)
  : _g{g}, _scope{scope} {
}

Extractor::~Extractor() noexcept {
}

void Extractor::visit(ast::Module& node) {
  for (auto& item : node.sequence) {
    item->accept(*this);
  }
}

void Extractor::visit(ast::Function& node) {
  auto& name = node.name->text;

  // FIXME: auto type = resolve(node)
  auto type = LLVMFunctionType(LLVMVoidType(), nullptr, 0, false);
  auto handle = LLVMAddFunction(_g._mod, name.c_str(), type);

  // Create and set the new function item in the scope
  // TODO: Forward spans
  _scope.set(name, std::make_shared<code::Function>(
    handle,
    name
  ));
}
