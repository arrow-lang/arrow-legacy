#ifndef ARROW_GENERATOR_H
#define ARROW_GENERATOR_H 1

#include "arrow/tokenizer.hpp"
#include "arrow/parser.hpp"

extern "C" {
#include "llvm-c/Core.h"
#include "llvm-c/Target.h"
#include "llvm-c/TargetMachine.h"
}

namespace arrow {

  class Generator {
  public:
    Generator();

    Generator(const Generator&) = delete;

    ~Generator() noexcept;

    void operator=(const Generator&) = delete;

    void generate(const std::string& name, std::shared_ptr<ast::Node> node);

    void print(std::ostream& os) const;

  private:
    /// LLVM module that contains the LLVM IR
    LLVMModuleRef _mod;

    /// LLVM instruction builder that greatly simplifies
    /// IR generation with LLVM
    LLVMBuilderRef _irb;

    /// LLVM target machine
    LLVMTargetMachineRef _target_machine;

    /// LLVM target data (layout)
    LLVMTargetDataRef _data_layout;

  };

}

#endif // ARROW_GENERATOR_H
