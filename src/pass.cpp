// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/build.hpp"
#include "arrow/pass/expose.hpp"
#include "arrow/pass/analyze.hpp"
#include "arrow/pass/type.hpp"

#define IMPL(N) \
  N::~N() noexcept { }

namespace arrow {
namespace pass {

IMPL(Build)
IMPL(Expose)
IMPL(Analyze)
IMPL(Type)

}  // namespace pass
}  // namespace arrow
