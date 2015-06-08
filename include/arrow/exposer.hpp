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

class Exposer : public ast::Visitor {
 public:
  Exposer(Generator& g, code::Scope& _scope);

  virtual ~Exposer() noexcept;

  virtual void visit_module(ast::Module&);
  virtual void visit_function(ast::Function&);
  virtual void visit_extern_function(ast::ExternalFunction&);
  virtual void visit_import(ast::Import&);
  virtual void visit_struct(ast::Structure&);

 private:
  Generator& _g;
  code::Scope& _scope;
};

}  // namespace arrow

#endif  // ARROW_EXPOSE_H
