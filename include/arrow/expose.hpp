// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_EXPOSE_H
#define ARROW_EXPOSE_H 1

#include <stack>
#include <vector>

#include "arrow/ast.hpp"
#include "arrow/code.hpp"

namespace arrow {

class Generator;

class Expose : public ast::Visitor {
 public:
  Expose(Generator& g, code::Scope& _scope);

  virtual ~Expose() noexcept;

  virtual void visit_function(ast::Function&);

 private:
  Generator& _g;
  code::Scope& _scope;
};

}  // namespace arrow

#endif  // ARROW_EXPOSE_H
