// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_PASS_TYPE_H
#define ARROW_PASS_TYPE_H 1

#include "arrow/ast.hpp"
#include "arrow/code.hpp"

namespace arrow {
namespace pass {

class Type : public ast::Visitor {
 public:
  explicit Type(Ref<code::Scope> scope)
      : _scope(scope) {
  }

  virtual ~Type() noexcept;

  Ref<code::Type> run(ast::Node& x) {
    ast::Visitor::run(x);

    return _stack.size() > 0 ? _stack.front() : nullptr;
  }

  virtual void visit_type_path(ast::TypePath&);
  virtual void visit_type_tuple(ast::TypeTuple&);

 private:
  // The scope to emplace the exposed items into.
  Ref<code::Scope> _scope;

  std::deque<Ref<code::Type>> _stack;
};

}  // namespace pass
}  // namespace arrow

#endif  // ARROW_PASS_TYPE_H
