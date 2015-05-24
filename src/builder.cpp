// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/builder.hpp"
#include "arrow/resolver.hpp"
#include "arrow/generator.hpp"
#include "arrow/log.hpp"

using arrow::Builder;
using arrow::resolve;
namespace code = arrow::code;
namespace ast = arrow::ast;

Builder::Builder(arrow::Generator& g, code::Scope& scope)
  : _g{g}, _scope{scope}, _cs{&scope}, _stack{} {
}

Builder::~Builder() noexcept {
}

// Function
// -----------------------------------------------------------------------------
void Builder::visit(ast::Function& node) {
  auto& name = node.name->text;
  auto item = std::static_pointer_cast<code::Function>(_cs->get(name));

  auto block = LLVMAppendBasicBlock(item->handle(), "");
  LLVMPositionBuilderAtEnd(_g._irb, block);

  for (auto& el : node.sequence) {
    build(*el, &item->scope);
  }

  // Has the function been terminated?
  auto last = LLVMGetLastBasicBlock(item->handle());
  if (!LLVMGetBasicBlockTerminator(last)) {
    // No; we need to terminate
    if (!item->type()->as<code::FunctionType>().result) {
      // No result type
      LLVMBuildRetVoid(_g._irb);
    } else {
      // We should have gotten a result
      // TODO: Report an error
      return;
    }
  }

}

// Identifier
// -----------------------------------------------------------------------------
void Builder::visit(ast::Identifier& node) {
  auto item = _cs->get(node.text);
  if (item == nullptr) {
    return;
  }

  _stack.push(item);
}
