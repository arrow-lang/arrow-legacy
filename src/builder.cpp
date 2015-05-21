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

  LLVMBuildRetVoid(_g._irb);
}

// Call
// -----------------------------------------------------------------------------
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

// Identifier
// -----------------------------------------------------------------------------
void Builder::visit(ast::Identifier& node) {
  auto item = _cs->get(node.text);
  if (item == nullptr) {
    Log::get().error(
      node.span, "use of unresolved name `%s`", node.text.c_str());

    return;
  }

  _stack.push(item);
}

// Slot
// -----------------------------------------------------------------------------
void Builder::visit(ast::Slot& x) {
  auto& name = x.name->text;

  // Check if we are overwriting an item in the current scope
  // NOTE: We need to eventually decide if we will allow this or not
  //  I'm leaning towards allowing as long as we output a warning for
  //  an unused variable
  if (_cs->exists(name, false)) {
    Log::get().warning(x.name->span, "redefinition of '%s'", name.c_str());
  }

  // Build the initializer expression
  // TODO: Only if we have one
  auto initializer_item = build_scalar(*x.initializer);
  if (!initializer_item) return;
  if (!initializer_item->is_value()) {
    Log::get().error(x.initializer->span, "expected value");
    return;
  }

  auto initializer = std::static_pointer_cast<code::Value>(initializer_item);
  auto type = initializer->type();

  std::printf("Builder::viist(ast::Slot) %p\n", type.get());

  if (x.type != nullptr) {
    auto type_item = build_scalar(*x.type);
    if (!type_item) return;
    if (!type_item->is_type()) {
      Log::get().error(x.type->span, "expected typename");
      return;
    }

    // TODO: Check for type mis-match

    // Build the slot decl with the code generator
    type = std::static_pointer_cast<code::Type>(type_item);
  }

  auto handle = LLVMBuildAlloca(_g._irb, type->handle(), name.c_str());

  // Create and set the new slot decl in
  // the current scope
  _cs->set(name, std::make_shared<code::Slot>(
    name,
    handle,
    type));

  if (initializer) {
    // TODO: Extract into an `assign` function
    // Create a store for the initializer
    LLVMBuildStore(_g._irb, initializer->value_of(_g), handle);
  }
}
