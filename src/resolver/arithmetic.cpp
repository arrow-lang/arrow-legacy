// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/resolver.hpp"
#include "arrow/generator.hpp"
#include "arrow/log.hpp"

using arrow::Resolver;
using arrow::resolve;

void Resolver::do_arithmetic(ast::Binary& x) {
  // Resolve the types of both LHS and RHS
  auto lhs_type = resolve(_g, _scope, *x.lhs);
  auto rhs_type = resolve(_g, _scope, *x.rhs);
  if (!lhs_type || !rhs_type) return;

  // Check for special operations
  std::shared_ptr<code::Type> type = nullptr;
  if (lhs_type->is<code::PointerType>() &&
      rhs_type->is<code::IntegerType>()) {
    // ptr + int OR ptr - int
    if (x.is<ast::Add>() || x.is<ast::Sub>()) {
      type = lhs_type;
    }
  } else if (rhs_type->is<code::PointerType>() &&
             lhs_type->is<code::IntegerType>()) {
    // int + ptr (communative)
    if (x.is<ast::Add>()) {
      type = rhs_type;
    }
  } else if (rhs_type->is<code::PointerType>() &&
             lhs_type->is<code::PointerType>()) {
    // ptr - ptr
    if (x.is<ast::Sub>()) {
      // Get the size of an address on this processor
      // TODO: Should be pushed somewhere I can grab
      auto ptrsize = LLVMPointerSize(_g._data_layout) * 8;
      type = std::make_shared<code::IntegerType>(
        nullptr, nullptr, ptrsize, false);
    }
  }

  if (!type || (
      (lhs_type->is<code::IntegerType>() || lhs_type->is<code::FloatType>()) &&
      (rhs_type->is<code::IntegerType>() || rhs_type->is<code::FloatType>())
  )) {
    // Fallback and attempt to find a common type between the lhs and rhs
    type = common_type(x.lhs, x.rhs);
  }

  // Validate that the operation is actually supported between the two types
  // TODO: This obviously needs to change once we support overloading these
  if (!type) {
    // Resolve the individual types
    auto lhs_type = resolve(_g, _scope, *x.lhs);
    auto rhs_type = resolve(_g, _scope, *x.rhs);
    if (!lhs_type || !rhs_type) return;

    // Report that we don't support arithmetic operators for you
    std::string op;
    if (x.is<ast::Add>()) {
      op = "+";
    } else if (x.is<ast::Sub>()) {
      op = "-";
    } else if (x.is<ast::Mul>()) {
      op = "*";
    } else if (x.is<ast::Div>()) {
      op = "/";
    } else if (x.is<ast::Mod>()) {
      op = "%";
    }

    Log::get().error(x.span,
      "cannot apply binary operator `%s` to types '%s' and '%s'",
      op.c_str(),
      lhs_type->name().c_str(),
      rhs_type->name().c_str());

    return;
  }

  _stack.push(type);
}

void Resolver::visit_add(ast::Add& x) {
  do_arithmetic(x);
}

void Resolver::visit_sub(ast::Sub& x) {
  do_arithmetic(x);
}

void Resolver::visit_mul(ast::Mul& x) {
  do_arithmetic(x);
}

void Resolver::visit_div(ast::Div& x) {
  do_arithmetic(x);
}

void Resolver::visit_mod(ast::Mod& x) {
  do_arithmetic(x);
}
