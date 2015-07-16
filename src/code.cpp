// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/code.hpp"

#define IMPL(N) \
  N::~N() noexcept { }

namespace arrow {
namespace code {

IMPL(Item)
IMPL(Module)
IMPL(Slot)
IMPL(ExternSlot)
IMPL(Typename)

IMPL(Type)
IMPL(TypeNone)
IMPL(TypeBoolean)
IMPL(TypeFloat)
IMPL(TypeInteger)
IMPL(TypeByte)
IMPL(TypeSizedInteger)
IMPL(TypeTuple)

IMPL(Value)
IMPL(ValueNone)
IMPL(ValueTuple)

}  // namespace code
}  // namespace arrow
