// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/code.hpp"

#define IMPL(N) \
  N::~N() noexcept { }

namespace arrow {
namespace code {

IMPL(Item)
IMPL(Slot)
IMPL(Typename)

IMPL(Type)
IMPL(BooleanType)
IMPL(FloatType)
IMPL(IntegerType)

}  // namespace code
}  // namespace arrow
