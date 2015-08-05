// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/pass/analyze-type.hpp"
#include "arrow/util.hpp"

namespace arrow {
namespace pass {

void AnalyzeType::visit_import(ast::Import& x) {
  // Get the existing import-item
  auto current_module = util::current_module(_scope);
  for (auto& imp_item : current_module->imports) {
    if (imp_item->context == &x) {
      imp_item->module->context->accept(*this);
      break;
    }
  }
}

}  // namespace pass
}  // namespace arrow
