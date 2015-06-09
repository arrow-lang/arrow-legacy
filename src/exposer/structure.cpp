// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/exposer.hpp"
#include "arrow/resolver.hpp"
#include "arrow/generator.hpp"
#include "arrow/log.hpp"

using arrow::resolve;
using arrow::Exposer;
namespace code = arrow::code;
namespace ast = arrow::ast;

void Exposer::visit_struct(ast::Structure& x) {
  // Resolve the type of this struct
  auto type = std::static_pointer_cast<code::StructureType>(
    resolve(_g, _scope, x));
  if (!type) { return; }

  // Create and set the new structure item in the scope
  _scope.set(x.name->text, std::make_shared<code::Structure>(
    &x,
    &_scope,
    x.name->text,
    type));
}
