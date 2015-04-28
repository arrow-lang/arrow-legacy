#include "arrow/generator.hpp"
#include "arrow/builder.hpp"

using arrow::Generator;

Generator::Generator()
  : _mod{nullptr},
    _irb{nullptr},
    _target_machine{nullptr},
    _data_layout{nullptr},
    _scope{}
{
}

Generator::~Generator() noexcept
{
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
  const std::string& name, std::shared_ptr<ast::Node> node)
{
  // Ensure the x86 target is initialized.
  // NOTE: We should first ask configuration what our target is
  //   and attempt to initialize the right target.
  LLVMInitializeX86Target();
  LLVMInitializeX86TargetInfo();

  // Construct a LLVM module to hold the geneated IR.
  _mod = LLVMModuleCreateWithName(name.c_str());

  // Discern the triple for our target machine.
  // TODO: This should be a configuration option.
  // FIXME: At the very least this should be output with a verbose flag
  //        for debugging.
  auto triple = LLVMGetDefaultTargetTriple();
  LLVMTargetRef target = nullptr;
  char* error = nullptr;
  if (LLVMGetTargetFromTriple(triple, &target, &error) != 0) {
    // Failed to get a valid target
    // TODO: Report error
    return;
  }

  // Construct the target machine
  _target_machine = LLVMCreateTargetMachine(
    target, triple, "", "",
    LLVMCodeGenLevelDefault,
    LLVMRelocDefault,
    LLVMCodeModelDefault
  );

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

  // Construct a intermediate builder and begin the code generation
  // process on the passed node
  Builder builder{*this, _scope};
  node->accept(builder);
}

void Generator::print(std::ostream& os) const
{
  // Output the generated LLVM IR.
  auto bytes = LLVMPrintModuleToString(_mod);

  os << bytes;

  LLVMDisposeMessage(bytes);
}
