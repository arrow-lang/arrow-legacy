// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <boost/filesystem.hpp>

#include "arrow/compiler.hpp"
#include "arrow/pass/build.hpp"
#include "arrow/pass/expose.hpp"
#include "arrow/pass/analyze-type.hpp"
#include "arrow/pass/analyze-module.hpp"
#include "arrow/pass/analyze-usage.hpp"
#include "arrow/pass/declare.hpp"
#include "arrow/pass/define.hpp"

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

  // String
  _scope->insert(
    new code::Typename(nullptr, "str", new code::TypeString()));

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

  // Create (and emplace) the top-level module item
  Ref<code::Module> top = new code::Module(node, name, nullptr, _scope);
  auto pathname = fs::canonical(fs::absolute(node->span.filename)).string();
  _ctx.modules.push_back(top);
  _ctx.modules_by_context[node.get()] = top;
  _ctx.modules_by_pathname[pathname] = top;

  // Invoke the initial pass: expose
  pass::Expose(_ctx, _scope).run(*node);
  if (Log::get().count("error") > 0) return;

  // Iterate through each now-exposed module and invoke each subsequent pass
  // Analyze Type
  pass::AnalyzeType(_ctx, _scope).run(*node);
  if (Log::get().count("error") > 0) return;

  // Analyze Usage
  for (auto& item : _ctx.modules) {
    pass::AnalyzeUsage(_ctx, _scope).run(*item->context);
    if (Log::get().count("error") > 0) return;
  }

  // Analyze Module(s)
  for (auto& item : _ctx.modules) {
    pass::AnalyzeModule(_ctx, _scope).run(*item->context);
    if (Log::get().count("error") > 0) return;
  }

  // Analyze module dependency graph
  // A module cannot /depend/ on the top-level module; we provide
  // a guarantee that the top-level (or entry) module is ran last.
  for (auto& item : _ctx.modules) {
    if (item.get() != top.get()) {
      for (auto& dependency : item->dependencies) {
        if (dependency == top.get()) {
          Log::get().error("cannot depend on the entry module\n");
        }
      }
    }
  }

  // Finally; modules must be able to be ordered. There cannot be
  // circular dependencies.
  std::deque<code::Module*> ordered_modules;
  std::deque<code::Module*> incoming;
  std::unordered_map<code::Module*, std::unordered_set<code::Module*>> graph;

  for (auto& module : _ctx.modules) {
    if (module.get() == top.get()) continue;
    if (module->dependencies.size() == 0) {
      incoming.push_back(module.get());
    } else {
      graph.emplace(module.get(), module->dependencies);

      // Ensure we don't have a dependency to ourself (this doesn't matter)
      graph[module.get()].erase(module.get());
    }
  }

  while (!incoming.empty()) {
    auto mod = incoming.front();
    incoming.pop_front();
    ordered_modules.push_back(mod);

    std::deque<code::Module*> collect;
    for (auto& node : graph) {
      node.second.erase(mod);
      if (node.second.size() == 0) {
        collect.push_back(node.first);
      }
    }

    for (auto& mod_item : collect) {
      graph.erase(mod_item);
      incoming.push_back(mod_item);
    }
  }

  if (graph.size() > 0) {
    for (auto& node : graph) {
      for (auto& mod : node.second) {
        // Find the <Import> For this module
        for (auto& imp : node.first->imports) {
          if (imp->module.get() == mod) {
            // Display the error message
            Log::get().error(
              imp->context->span,
              "unresolvable circular dependency with module: '%s'",
              mod->name.c_str());

            break;
          }
        }
      }
    }
  }

  // Declare
  for (auto& item : _ctx.modules) {
    pass::Declare(_ctx, _scope).run(*item->context);
    if (Log::get().count("error") > 0) return;
  }

  // Define
  for (auto& item : _ctx.modules) {
    pass::Define(_ctx, _scope).run(*item->context);
    if (Log::get().count("error") > 0) return;
  }

  // Build
  for (auto& item : _ctx.modules) {
    pass::Build(_ctx, _scope).run(*item->context);
    if (Log::get().count("error") > 0) return;
  }

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

  // Build calls to each imported modules' module initializer
  for (auto& module : ordered_modules) {
    LLVMBuildCall(_ctx.irb, module->initializer, nullptr, 0, "");
  }

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
