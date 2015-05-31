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
  if (!item) { return; }

  auto type = item->type();

  auto block = LLVMAppendBasicBlock(item->handle(), "");
  LLVMPositionBuilderAtEnd(_g._irb, block);

  // Set us as the active function (so return statements know who
  // to return from)
  auto of = _cf;
  _cf = item.get();

  // Allocate the parameter nodes into the local scope.
  for (unsigned index = 0; index < node.parameters.size(); ++index) {
    auto& param = node.parameters.at(index);
    auto& param_type = type->parameters.at(index);

    // Allocate space on the stack for this parameter
    // TODO: The parameter name should be bound to the type
    auto param_handle = LLVMBuildAlloca(
      _g._irb, param_type->handle(), param->name->text.c_str());

    // Store the parameter in the allocation.
    LLVMBuildStore(_g._irb,
      LLVMGetParam(item->handle(), index), param_handle);

    // Insert into the local scope.
    item->scope.set(param->name->text.c_str(), std::make_shared<code::Slot>(
      param->name->text,
      param_handle,
      param_type,
      false
    ));
  }

  // Iterate and build each node in the function sequence
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
    // TODO: Should really check if errors occured for this function
    } else if (Log::get().count("error") == 0) {
      // We should have gotten a result; report an error and exit
      Log::get().error(node.span, "not all code paths return a value");
      return;
    }
  }

  // Release the current function
  _cf = of;
}

// Identifier
// -----------------------------------------------------------------------------
void Builder::visit(ast::Identifier& node) {
  auto item = _cs->get(node.text);
  if (item == nullptr) {
    Log::get().error(
      node.span, "use of unresolved name '%s'", node.text.c_str());

    return;
  }

  _stack.push(item);
}
