// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <string>
#include "arrow/generator.hpp"
#include "arrow/builder.hpp"
#include "arrow/exposer.hpp"
#include "arrow/code.hpp"

using arrow::Generator;

Generator::Generator()
  : _mod{nullptr},
    _irb{nullptr},
    _target_machine{nullptr},
    _data_layout{nullptr},
    _scope{""} {
}

Generator::~Generator() noexcept {
  if (_mod) {
    // Dispose of the LLVM module.
    LLVMDisposeModule(_mod);
  }

  if (_irb) {
    // Dispose of the instruction builder.
    LLVMDisposeBuilder(_irb);
  }

  if (_target_machine) {
    // Dispose of the target machine.
    LLVMDisposeTargetMachine(_target_machine);
  }
}

void Generator::generate(
  const std::string& name, std::shared_ptr<ast::Node> node
) {
  // Ensure the x86 target is initialized.
  // NOTE: We should first ask configuration what our target is
  //   and attempt to initialize the right target.
  LLVMInitializeX86Target();
  LLVMInitializeX86TargetInfo();

  // Construct a LLVM module to hold the geneated IR.
  _mod = LLVMModuleCreateWithName(name.c_str());
  _name = name;

  // Discern the triple for our target machine.
  auto triple = LLVMGetDefaultTargetTriple();
  LLVMTargetRef target = nullptr;
  char* error = nullptr;
  if (LLVMGetTargetFromTriple(triple, &target, &error) != 0) {
    // Failed to get a valid target
    return;
  }

  // Construct the target machine
  _target_machine = LLVMCreateTargetMachine(
    target, triple, "", "",
    LLVMCodeGenLevelDefault,
    LLVMRelocDefault,
    LLVMCodeModelDefault);

  // Set the target triple (on the IR module)
  LLVMSetTarget(_mod, triple);

  // Get and set the data layout (on the IR module)
  _data_layout = LLVMGetTargetMachineData(_target_machine);
  auto data_layout_text = LLVMCopyStringRepOfTargetData(_data_layout);
  LLVMSetDataLayout(_mod, data_layout_text);

  // Dispose of the used messages
  LLVMDisposeMessage(triple);
  LLVMDisposeMessage(data_layout_text);

  // Construct the instruction builder
  _irb = LLVMCreateBuilder();

  // Declare the basic (built-in) types
  _declare_basic_types();

  // Construct the exposer and run it over the top-level module
  arrow::Exposer{*this, _scope}.run(*node);
  if (Log::get().count("error") > 0) { return; }

  // Construct an intermediate builder and run it on each
  // now-exposed module
  for (auto& mod : _modules) {
    arrow::Builder builder{*this, _scope};
    for (auto& item : dynamic_cast<ast::Module*>(mod->context)->sequence) {
      builder.build(*item, &(mod->scope));
    }
  }
}

void Generator::print(std::ostream& os) const {
  // Output the generated LLVM IR.
  auto bytes = LLVMPrintModuleToString(_mod);

  os << bytes;

  LLVMDisposeMessage(bytes);
}

void Generator::_declare_basic_types() {
  // Basic types

  // Boolean
  _scope.set("bool", std::make_shared<code::BooleanType>(nullptr, nullptr));

  // Numeric
  _scope.set("byte", std::make_shared<code::IntegerType>(nullptr, nullptr, 8, false));
  _scope.set("int", std::make_shared<code::IntegerType>(nullptr, nullptr, 64, true));
  _scope.set("float", std::make_shared<code::FloatType>(nullptr, nullptr, 64));

  // UTF-8 encoded, UTF-32 string
  _scope.set("str", std::make_shared<code::StringType>(nullptr, nullptr));

  // Storage (focused) types

  // Signed, machine-independent integer types
  _scope.set("int8", std::make_shared<code::IntegerType>(
      nullptr, nullptr, 8, true));
  _scope.set("int16", std::make_shared<code::IntegerType>(
      nullptr, nullptr, 16, true));
  _scope.set("int32", std::make_shared<code::IntegerType>(
      nullptr, nullptr, 32, true));
  _scope.set("int64", std::make_shared<code::IntegerType>(
      nullptr, nullptr, 64, true));
  _scope.set("int128", std::make_shared<code::IntegerType>(
      nullptr, nullptr, 128, true));

  // Unsigned, machine-independent integer types
  _scope.set("uint8", std::make_shared<code::IntegerType>(
      nullptr, nullptr, 8, false));
  _scope.set("uint16", std::make_shared<code::IntegerType>(
      nullptr, nullptr, 16, false));
  _scope.set("uint32", std::make_shared<code::IntegerType>(
      nullptr, nullptr, 32, false));
  _scope.set("uint64", std::make_shared<code::IntegerType>(
      nullptr, nullptr, 64, false));
  _scope.set("uint128", std::make_shared<code::IntegerType>(
      nullptr, nullptr, 128, false));

  // Floating-point types
  _scope.set("float32", std::make_shared<code::FloatType>(
      nullptr, nullptr, 32));
  _scope.set("float64", std::make_shared<code::FloatType>(
      nullptr, nullptr, 64));
}

void Generator::generate_main() {
  // Get a reference to our top-level module main (if present)
  auto mod = std::dynamic_pointer_cast<code::Module>(_scope.get(_name));
  if (!mod) return;

  auto mod_main_item = mod->scope.get("main");
  std::shared_ptr<code::Function> mod_main = nullptr;
  if (mod_main_item) {
    mod_main = std::dynamic_pointer_cast<code::Function>(mod_main_item);
    if (!mod_main) {
      // TODO: No idea where this came from; we should save context
      //       on decl. items
      Log::get().error("'main' must be a function");
    }

    // Check for no return type or the correct return type
    auto mm = mod_main->type();
    if (mm->result && !(mm->result->is<code::IntegerType>() && mm->result->as<code::IntegerType>().bits == 32)) {
      Log::get().error("result of `main` must be unit or `int32`");
    }

    // Check for the correct parameter count
    if (mm->parameters.size() > 3) {
      // TODO: Waiting on context to understand where this came from
      Log::get().error(
        "too many parameters (%d) for 'main': must be 0, 2, or 3",
        mm->parameters.size());

    }

    if (mm->parameters.size() == 1) {
      // TODO: Waiting on context to understand where this came from
      Log::get().error(
        "only one parameter for 'main': must be 0, 2, or 3");

    }

    bool param_type_match = false;

    if (mm->parameters.size() >= 1) {
      // TODO: Revisit when we understand `c_int`
      if (mm->parameters[0]->is<code::IntegerType>()) {
        if (mm->parameters[0]->as<code::IntegerType>().bits == 32) {
          param_type_match = true;
        }
      }
      if (!param_type_match) {
        // TODO: Waiting on context to understand where this came from
        Log::get().error(
          "first parameter of 'main' (argc) must be of type 'int32'");
      }
    }

    if (mm->parameters.size() >= 2) {
      if (mm->parameters[1]->is<code::PointerType>()) {
        if (mm->parameters[1]->as<code::PointerType>().pointee->is<code::StringType>()) {
          param_type_match = true;
        }
      }
      if (!param_type_match) {
        // TODO: Waiting on context to understand where this came from
        Log::get().error(
          "second parameter of 'main' (argv) must be of type '*str'");
      }
    }

    if (mm->parameters.size() >= 3) {
      if (mm->parameters[2]->is<code::PointerType>()) {
        if (mm->parameters[2]->as<code::PointerType>().pointee->is<code::StringType>()) {
          param_type_match = true;
        }
      }
      if (!param_type_match) {
        // TODO: Waiting on context to understand where this came from
        Log::get().error(
          "third parameter of 'main' (environ) must be of type '*str'");
      }
    }
  }

  // Do we keep going ?
  if (Log::get().count("error") > 0) { return; }

  // Build the ABI main function (declaration)
  // NOTE: The correct definition is: `int main(int, **int8, **int8)`
  // TODO: Revisit when we /know/ what type `c_int` is
  std::vector<LLVMTypeRef> abi_main_params{
    LLVMInt32Type(),
    LLVMPointerType(LLVMPointerType(LLVMInt8Type(), 0), 0),
    LLVMPointerType(LLVMPointerType(LLVMInt8Type(), 0), 0)};
  auto abi_main_ty = LLVMFunctionType(
    LLVMInt32Type(), abi_main_params.data(), abi_main_params.size(), 0);
  auto abi_main = LLVMAddFunction(_mod, "main", abi_main_ty);

  // Build the ABI main function (definition)
  LLVMPositionBuilderAtEnd(_irb, LLVMAppendBasicBlock(abi_main, ""));

  if (mod_main) {
    // Collect arguments to call the module main function
    std::vector<LLVMValueRef> main_args;
    for (unsigned idx = 0; idx < mod_main->type()->parameters.size(); ++idx) {
      main_args.push_back(LLVMGetParam(abi_main, idx));
    }

    // Build a call to the module main
    auto main_res = LLVMBuildCall(
      _irb, mod_main->handle(*this), main_args.data(), main_args.size(), "");

    // If the module main returns ..
    if (mod_main->type()->result) {
      LLVMBuildRet(_irb, main_res);
    }
  }

  // If we didn't terminate ..
  if (!LLVMGetBasicBlockTerminator(LLVMGetLastBasicBlock(abi_main))) {
    LLVMBuildRet(_irb, LLVMConstInt(LLVMInt32Type(), 0, false));
  }
}
