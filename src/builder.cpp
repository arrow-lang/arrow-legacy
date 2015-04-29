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

void Builder::visit(ast::Slot& node) {
  auto& name = node.name->text;

  // Ensure that we are not overwriting an item in the current scope
  if (_cs->exists(name, false)) {
    Log::get().warning("redefinition of '%s'", name.c_str());
  }

  auto type_item = build_scalar(*node.type);
  if (!type_item) return;
  if (!type_item->is_type()) {
    // TODO: Report location
    Log::get().error("expected type name");
    return;
  }


  // Build the slot decl with the code generator
  auto type = std::static_pointer_cast<code::Type>(type_item);
  auto handle = LLVMBuildAlloca(_g._irb, type->handle(), name.c_str());

  // Create and set the new slot decl in
  // the current scope
  _cs->set(name, std::make_shared<code::Slot>(
    handle,
    name
  ));
}
