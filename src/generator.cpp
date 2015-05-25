#include "arrow/generator.hpp"
#include "arrow/builder.hpp"
#include "arrow/code.hpp"

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
  const std::string& name, std::shared_ptr<ast::Node> node
) {
  // Ensure the x86 target is initialized.
  // NOTE: We should first ask configuration what our target is
  //   and attempt to initialize the right target.
  LLVMInitializeX86Target();
  LLVMInitializeX86TargetInfo();

  // Construct a LLVM module to hold the geneated IR.
  _mod = LLVMModuleCreateWithName(name.c_str());

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

  // Construct a intermediate builder and begin the code generation
  // process on the passed node
  Builder builder{*this, _scope};
  node->accept(builder);
}

void Generator::print(std::ostream& os) const {
  // Output the generated LLVM IR.
  auto bytes = LLVMPrintModuleToString(_mod);

  os << bytes;

  LLVMDisposeMessage(bytes);
}

void Generator::_declare_basic_types() {
  // Boolean type
  _scope.set("bool", std::make_shared<code::BooleanType>());

  // Signed, machine-independent integer types
  _scope.set("int8", std::make_shared<code::IntegerType>(8, true));
  _scope.set("int16", std::make_shared<code::IntegerType>(16, true));
  _scope.set("int32", std::make_shared<code::IntegerType>(32, true));
  _scope.set("int64", std::make_shared<code::IntegerType>(64, true));
  _scope.set("int128", std::make_shared<code::IntegerType>(128, true));

  // Unsigned, machine-independent integer types
  _scope.set("uint8", std::make_shared<code::IntegerType>(8, false));
  _scope.set("uint16", std::make_shared<code::IntegerType>(16, false));
  _scope.set("uint32", std::make_shared<code::IntegerType>(32, false));
  _scope.set("uint64", std::make_shared<code::IntegerType>(64, false));
  _scope.set("uint128", std::make_shared<code::IntegerType>(128, false));

  // Floating-point types
  _scope.set("float32", std::make_shared<code::FloatType>(32));
  _scope.set("float64", std::make_shared<code::FloatType>(64));

  // TODO: Machine-dependent integer types
  // TODO: UTF-32 character type

  // UTF-8 string type
  _scope.set("str", std::make_shared<code::StringType>());

  // TODO: Byte string type
}
