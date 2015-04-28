// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/builder.hpp"
#include "arrow/generator.hpp"
#include "arrow/log.hpp"

using arrow::Builder;
namespace code = arrow::code;
namespace ast = arrow::ast;

Builder::Builder(arrow::Generator& g, code::Scope& scope)
  : _g{g}, _scope{scope}, _cs{&scope}, _stack{} {
}

Builder::~Builder() noexcept {
}

void Builder::visit(ast::Function& node) {
  auto& name = node.name->text;
  auto item = std::static_pointer_cast<code::Function>(_cs->get(name));
  if (item == nullptr) {
    // TODO: Report error (?)
    return;
  }

  auto block = LLVMAppendBasicBlock(item->handle(), "");
  LLVMPositionBuilderAtEnd(_g._irb, block);

  for (auto& item : node.sequence) {
    item->accept(*this);
  }

  LLVMBuildRetVoid(_g._irb);
}

void Builder::visit(ast::Call& node) {
  auto item = std::static_pointer_cast<code::Function>(
    build_scalar(*node.expression));

  if (item == nullptr) {
    return;
  }

  // if (!item->is_function()) { }

  auto handle = item->handle();

  LLVMBuildCall(_g._irb, handle, nullptr, 0, "");
}

void Builder::visit(ast::Identifier& node) {
  auto item = _cs->get(node.text);
  if (item == nullptr) {
    // TODO: Report location
    Log::get().error("use of unresolved name `%s`", node.text.c_str());
    return;
  }

  _stack.push(item);
}
