// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <boost/filesystem.hpp>

#include "arrow/compiler.hpp"
#include "arrow/pass/build.hpp"

namespace fs = boost::filesystem;

namespace arrow {

Compiler::Compiler(bool verify) : _scope(new code::Scope("")), _verify(verify) {
  // Create a built-in scope-block
  _scope->enter(nullptr);
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
  LLVMInitializeNativeTarget();
  LLVMInitializeX86Target();
  LLVMInitializeX86TargetInfo();
  LLVMInitializeNativeAsmPrinter();

  // Declare builtin types

  // Boolean
  _scope->insert(
    new code::Typename(nullptr, "bool", new code::TypeBoolean()));

  // Byte
  _scope->insert(
    new code::Typename(nullptr, "byte", new code::TypeByte()));

  // Integer
  // TODO(mehcode): `int` to be variable percision
  _scope->insert(
    new code::Typename(nullptr, "int", new code::TypeInteger()));

  // Float
  _scope->insert(
    new code::Typename(nullptr, "float", new code::TypeFloat()));

  // Sized Integers
  _scope->insert(
    new code::Typename(
    nullptr, "int8", new code::TypeSizedInteger(8)));
  _scope->insert(
    new code::Typename(
    nullptr, "int16", new code::TypeSizedInteger(16)));
  _scope->insert(
    new code::Typename(
    nullptr, "int32", new code::TypeSizedInteger(32)));
  _scope->insert(
    new code::Typename(
    nullptr, "int64", new code::TypeSizedInteger(64)));
  _scope->insert(
    new code::Typename(
    nullptr, "int128", new code::TypeSizedInteger(128)));

  _scope->insert(
    new code::Typename(
    nullptr, "uint8", new code::TypeSizedInteger(8, false)));
  _scope->insert(
    new code::Typename(
    nullptr, "uint16", new code::TypeSizedInteger(16, false)));
  _scope->insert(
    new code::Typename(
    nullptr, "uint32", new code::TypeSizedInteger(32, false)));
  _scope->insert(
    new code::Typename(
    nullptr, "uint64", new code::TypeSizedInteger(64, false)));
  _scope->insert(
    new code::Typename(
    nullptr, "uint128", new code::TypeSizedInteger(128, false)));
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
  if (Log::get().count("error") > 0) return;

  // Get a reference to our top-level module main (if present)
  auto top = _scope->find(name).as<code::Module>();
  if (!top) return;

  // Build the ABI main function (declaration)
  // NOTE: The correct definition is: `int main(int, **int8, **int8)`
  // TODO(mehcode): Revisit when we /know/ what type `c_int` is
  std::vector<LLVMTypeRef> abi_main_params{
    LLVMInt32Type(),
    LLVMPointerType(LLVMPointerType(LLVMInt8Type(), 0), 0),
    LLVMPointerType(LLVMPointerType(LLVMInt8Type(), 0), 0)};
  auto abi_main_ty = LLVMFunctionType(
    LLVMInt32Type(), abi_main_params.data(), abi_main_params.size(), 0);
  auto abi_main = LLVMAddFunction(_ctx.mod, "main", abi_main_ty);

  // Build the ABI main function (definition)
  LLVMPositionBuilderAtEnd(_ctx.irb, LLVMAppendBasicBlock(abi_main, ""));

  // TODO(mehcode): Build calls to each imported modules' module initializer

  // Build a call to the top-level module initializer
  LLVMBuildCall(_ctx.irb, top->initializer, nullptr, 0, "");

  // TODO(mehcode): Discover and build a call to the module main function

  // If we didn't terminate (by returning the value of module main) ..
  if (!LLVMGetBasicBlockTerminator(LLVMGetLastBasicBlock(abi_main))) {
    LLVMBuildRet(_ctx.irb, LLVMConstInt(LLVMInt32Type(), 0, false));
  }

  // Verify the built module
  if (_verify) {
    error = nullptr;
    LLVMVerifyModule(_ctx.mod, LLVMAbortProcessAction, &error);
    LLVMDisposeMessage(error);
  }
}

void Compiler::print() {
  // Output the generated LLVM IR.
  auto bytes = LLVMPrintModuleToString(_ctx.mod);
  std::printf("%s\n", bytes);
  LLVMDisposeMessage(bytes);
}

int Compiler::run(int argc, char** argv, char** environ) {
  // Create an execution engine ..
  char* error = nullptr;
  LLVMExecutionEngineRef engine = nullptr;
  LLVMCreateExecutionEngineForModule(&engine, _ctx.mod, &error);
  if (!engine) {
    LLVMDisposeMessage(error);
    return -1;
  }

  // Run static initialization ..
  LLVMRunStaticConstructors(engine);

  // Run `main`
  auto res = LLVMRunFunctionAsMain(
    engine,
    LLVMGetNamedFunction(_ctx.mod, "main"),
    argc, argv, environ);

  // Run static finalization ..
  LLVMRunStaticDestructors(engine);

  return res;
}

}  // namespace arrow
