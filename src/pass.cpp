// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/build.hpp"
#include "arrow/pass/expose.hpp"
#include "arrow/pass/type.hpp"
#include "arrow/pass/resolve.hpp"
#include "arrow/pass/analyze-usage.hpp"

#define IMPL(N) \
  N::~N() noexcept { }

namespace arrow {
namespace pass {

IMPL(Build)
IMPL(Expose)
IMPL(Type)
IMPL(Resolve)
IMPL(AnalyzeUsage)

}  // namespace pass
}  // namespace arrow
