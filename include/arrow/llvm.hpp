// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_LLVM_H
#define ARROW_LLVM_H 1

#include "llvm-c/Core.h"
#include "llvm-c/Target.h"
#include "llvm-c/TargetMachine.h"
#include "llvm-c/Analysis.h"

extern void LLVMAddResultAttribute(LLVMValueRef Fn, LLVMAttribute PA);
extern void LLVMRemoveResultAttribute(LLVMValueRef Fn, LLVMAttribute PA);
extern LLVMAttribute LLVMGetResultAttribute(LLVMValueRef Fn);

#endif  // ARROW_LLVM_H
