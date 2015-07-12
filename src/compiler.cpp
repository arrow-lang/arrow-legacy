// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <boost/filesystem.hpp>

#include "arrow/compiler.hpp"
#include "arrow/pass/build.hpp"

namespace fs = boost::filesystem;

namespace arrow {

Compiler::Compiler() : _scope(new code::Scope()) {
}

Compiler::~Compiler() noexcept {
  if (_ctx.mod) {
    // Dispose of the LLVM module.
    LLVMDisposeModule(_ctx.mod);
  }

  if (_ctx.irb) {
    // Dispose of the instruction builder.
    LLVMDisposeBuilder(_ctx.irb);
  }

  if (_ctx.target_machine) {
    // Dispose of the target machine.
    LLVMDisposeTargetMachine(_ctx.target_machine);
  }
}

void Compiler::initialize() {
  // Ensure the x86 target is initialized.
  // NOTE: We should first ask configuration what our target is
  //   and attempt to initialize the right target.
  LLVMInitializeX86Target();
  LLVMInitializeX86TargetInfo();

  // Declare builtin types

  // Boolean
  _scope->emplace(
    new code::Typename(nullptr, "bool", new code::TypeBoolean()));

  // Integer
  // TODO(mehcode): `int` to be variable percision
  _scope->emplace(
    new code::Typename(nullptr, "int", new code::TypeInteger(32)));

  // Float
  _scope->emplace(
    new code::Typename(nullptr, "float", new code::TypeFloat()));

  // Sized Integers
  _scope->emplace(
    new code::Typename(nullptr, "int8", new code::TypeInteger(8)));
  _scope->emplace(
    new code::Typename(nullptr, "int16", new code::TypeInteger(16)));
  _scope->emplace(
    new code::Typename(nullptr, "int32", new code::TypeInteger(32)));
  _scope->emplace(
    new code::Typename(nullptr, "int64", new code::TypeInteger(64)));
  _scope->emplace(
    new code::Typename(nullptr, "int128", new code::TypeInteger(128)));

  _scope->emplace(
    new code::Typename(nullptr, "uint8", new code::TypeInteger(8, false)));
  _scope->emplace(
    new code::Typename(nullptr, "uint16", new code::TypeInteger(16, false)));
  _scope->emplace(
    new code::Typename(nullptr, "uint32", new code::TypeInteger(32, false)));
  _scope->emplace(
    new code::Typename(nullptr, "uint64", new code::TypeInteger(64, false)));
  _scope->emplace(
    new code::Typename(nullptr, "uint128", new code::TypeInteger(128, false)));
}

void Compiler::compile(const std::string& name, Ref<ast::Node> node) {
  // Construct a LLVM module to hold the geneated IR.
  _ctx.mod = LLVMModuleCreateWithName(name.c_str());

  // Discern the triple for our target machine.
  auto triple = LLVMGetDefaultTargetTriple();
  LLVMTargetRef target = nullptr;
  char* error = nullptr;
  if (LLVMGetTargetFromTriple(triple, &target, &error) != 0) {
    // Failed to get a valid target
    return;
  }

  // Construct the target machine
  _ctx.target_machine = LLVMCreateTargetMachine(
    target, triple, "", "",
    LLVMCodeGenLevelDefault,
    LLVMRelocDefault,
    LLVMCodeModelDefault);

  // Set the target triple (on the IR module)
  LLVMSetTarget(_ctx.mod, triple);

  // Get and set the data layout (on the IR module)
  _ctx.data_layout = LLVMGetTargetMachineData(_ctx.target_machine);
  auto data_layout_text = LLVMCopyStringRepOfTargetData(_ctx.data_layout);
  LLVMSetDataLayout(_ctx.mod, data_layout_text);

  // Dispose of the used messages
  LLVMDisposeMessage(triple);
  LLVMDisposeMessage(data_layout_text);

  // Construct the instruction builder
  _ctx.irb = LLVMCreateBuilder();

  // Invoke the build pass on the given node (module)
  pass::Build(_ctx, _scope).run(*node);
}

void Compiler::print() {
  // Output the generated LLVM IR.
  auto bytes = LLVMPrintModuleToString(_ctx.mod);
  std::printf("%s\n", bytes);
  LLVMDisposeMessage(bytes);
}

}  // namespace arrow
