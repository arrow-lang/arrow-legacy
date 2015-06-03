// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <boost/property_tree/xml_parser.hpp>

#include "arrow/ast/visitor.hpp"

using arrow::ast::Visitor;
using arrow::ast::AbstractVisitor;

AbstractVisitor::~AbstractVisitor() noexcept { }

void AbstractVisitor::run(Node& node) {
  node.accept(*this);
}

Visitor::~Visitor() noexcept { }

#define DELEGATE(type, name) \
  void Visitor::visit(type& x) { visit_##name(x); }

DELEGATE(Identifier, id)
DELEGATE(Module, module)
DELEGATE(Function, function)
DELEGATE(ExternalFunction, extern_function)
DELEGATE(Parameter, parameter)
DELEGATE(Call, call)
DELEGATE(Slot, slot)
DELEGATE(Break, break)
DELEGATE(Return, return)
DELEGATE(Integer, int)
DELEGATE(Float, float)
DELEGATE(Boolean, bool)
DELEGATE(String, str)
DELEGATE(Promote, promote)
DELEGATE(NegateLogical, negate_logical)
DELEGATE(NegateBit, negate_bit)
DELEGATE(NegateNumeric, negate_numeric)
DELEGATE(Assign, assign)
DELEGATE(AssignAdd, assign_add)
DELEGATE(AssignSub, assign_sub)
DELEGATE(AssignMul, assign_mul)
DELEGATE(AssignDiv, assign_div)
DELEGATE(AssignMod, assign_mod)
DELEGATE(AssignBitAnd, assign_bit_and)
DELEGATE(AssignBitXor, assign_bit_xor)
DELEGATE(AssignBitOr, assign_bit_or)
DELEGATE(And, and)
DELEGATE(Or, or)
DELEGATE(EqualTo, eq)
DELEGATE(NotEqualTo, ne)
DELEGATE(LessThan, lt)
DELEGATE(LessThanOrEqualTo, le)
DELEGATE(GreaterThanOrEqualTo, ge)
DELEGATE(GreaterThan, gt)
DELEGATE(BitAnd, bit_and)
DELEGATE(BitXor, bit_xor)
DELEGATE(BitOr, bit_or)
DELEGATE(Add, add)
DELEGATE(Sub, sub)
DELEGATE(Mul, mul)
DELEGATE(Div, div)
DELEGATE(Mod, mod)
DELEGATE(Select, select)
DELEGATE(SelectBranch, select_branch)
DELEGATE(Loop, loop)

void Visitor::visit_module(Module& x) {
  for (auto& item : x.sequence) {
    item->accept(*this);
  }
}

void Visitor::visit_select(Select& x) {
  for (auto& item : x.branches) {
    item->accept(*this);
  }
}

void Visitor::visit_loop(Loop& x) {
  for (auto& item : x.sequence) {
    item->accept(*this);
  }
}

void Visitor::visit_select_branch(SelectBranch& x) {
  for (auto& item : x.sequence) {
    item->accept(*this);
  }
}
