#ifndef ARROW_GENERATOR_H
#define ARROW_GENERATOR_H 1

#include "arrow/tokenizer.hpp"
#include "arrow/parser.hpp"
#include "arrow/code.hpp"
#include "arrow/llvm.hpp"

namespace arrow {

  class Builder;
  class Extractor;

  class Generator {
  public:
    Generator();

    Generator(const Generator&) = delete;

    ~Generator() noexcept;

    void operator=(const Generator&) = delete;

    void generate(const std::string& name, std::shared_ptr<ast::Node> node);

    void print(std::ostream& os) const;

  private:
    friend class Builder;
    friend class Extractor;

    void _declare_basic_types();

    /// LLVM module that contains the LLVM IR
    LLVMModuleRef _mod;

    /// LLVM instruction builder that greatly simplifies
    /// IR generation with LLVM
    LLVMBuilderRef _irb;

    /// LLVM target machine
    LLVMTargetMachineRef _target_machine;

    /// LLVM target data (layout)
    LLVMTargetDataRef _data_layout;

    /// The builtin (above top-level) scope
    code::Scope _scope;

  };

}

#endif // ARROW_GENERATOR_H
