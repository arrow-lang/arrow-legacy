// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_PASS_MODULE_H
#define ARROW_PASS_MODULE_H 1

#include "arrow/ast.hpp"
#include "arrow/code.hpp"

namespace arrow {
namespace pass {

class Type : public ast::Visitor {
 public:
  Type(Ref<code::Scope> scope)
      : _scope(scope) {
  }

  virtual ~Type() noexcept;

  virtual void visit_id(ast::Identifier&);

 private:
  // The scope to emplace the exposed items into.
  Ref<code::Scope> _scope;
};

}  // namespace pass
}  // namespace arrow

#endif  // ARROW_PASS_MODULE_H
