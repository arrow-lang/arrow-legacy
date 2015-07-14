// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/build.hpp"
#include "arrow/pass/resolve.hpp"

namespace arrow {
namespace pass {

void Build::visit_tuple(ast::Tuple& x) {
  // Resolve the type of the literal boolean: `bool`
  auto type = Resolve(_scope).run(x);
  if (!type) return;

  // Iterate through the elements; and build each
  std::vector<LLVMValueRef> values;
  values.reserve(x.elements.size());
  for (auto& element : x.elements) {
    auto el_value = Build(_ctx, _scope).run_scalar(*element);
    if (!el_value) return;

    values.push_back(el_value->get_value(_ctx));
  }

  // Build the LLVM value
  // TODO(mehcode): non-constant elements
  auto handle = LLVMConstStruct(values.data(), values.size(), false);

  // Build and push the value
  _stack.push_front(new code::Value(handle, type));
}

}  // namespace pass
}  // namespace arrow
