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

Exposer::Exposer(arrow::Generator& g, code::Scope& scope)
  : _g(g), _scope(scope) {
}

Exposer::~Exposer() noexcept { }
