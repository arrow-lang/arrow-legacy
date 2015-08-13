// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/code.hpp"

#define IMPL(N) \
  N::~N() noexcept { }

namespace arrow {
namespace code {

IMPL(Item)
IMPL(Container)
IMPL(Module)
IMPL(Import)
IMPL(Slot)
IMPL(ExternSlot)
IMPL(Function)
IMPL(Parameter)
IMPL(Structure)
IMPL(ExternFunction)
IMPL(Typename)

IMPL(Type)
IMPL(TypeAny)
IMPL(TypeUnknown)
IMPL(TypeNone)
IMPL(TypeBoolean)
IMPL(TypeFloat)
IMPL(TypeInteger)
IMPL(TypeIntegerLiteral)
IMPL(TypeByte)
IMPL(TypeSizedInteger)
IMPL(TypeTuple)
IMPL(TypeFunction)
IMPL(TypeParameter)
IMPL(TypeString)
IMPL(TypePointer)
IMPL(TypeStructure)
IMPL(TypeMember)

IMPL(Value)
IMPL(ValueNone)
IMPL(ValueTuple)

}  // namespace code
}  // namespace arrow
