#ifndef ARROW_LLVM_H
#define ARROW_LLVM_H 1

#pragma push_macro("__cplusplus")
#undef __cplusplus

extern "C" {
#include "llvm-c/Core.h"
#include "llvm-c/Target.h"
#include "llvm-c/TargetMachine.h"
}

#pragma pop_macro("__cplusplus")

#endif // ARROW_LLVM_H
