// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/llvm.hpp"

// #include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/IR/Attributes.h"
// #include "llvm/IR/CallSite.h"
// #include "llvm/IR/Constants.h"
// #include "llvm/IR/DerivedTypes.h"
// #include "llvm/IR/DiagnosticInfo.h"
// #include "llvm/IR/DiagnosticPrinter.h"
// #include "llvm/IR/GlobalAlias.h"
// #include "llvm/IR/GlobalVariable.h"
// #include "llvm/IR/IRBuilder.h"
// #include "llvm/IR/InlineAsm.h"
// #include "llvm/IR/IntrinsicInst.h"
// #include "llvm/IR/LLVMContext.h"
// #include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
// #include "llvm/Support/Debug.h"
// #include "llvm/Support/ErrorHandling.h"
// #include "llvm/Support/FileSystem.h"
// #include "llvm/Support/ManagedStatic.h"
// #include "llvm/Support/MemoryBuffer.h"
// #include "llvm/Support/Threading.h"
// #include "llvm/Support/raw_ostream.h"

using namespace llvm;

void LLVMAddResultAttribute(LLVMValueRef Fn, LLVMAttribute PA) {
  Function *Func = unwrap<Function>(Fn);
  const AttributeSet PAL = Func->getAttributes();
  AttrBuilder B(PA);
  const AttributeSet PALnew =
    PAL.addAttributes(Func->getContext(), 0,
                      AttributeSet::get(Func->getContext(),
                                        0, B));
  Func->setAttributes(PALnew);
}
