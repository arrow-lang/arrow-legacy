// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <boost/property_tree/xml_parser.hpp>

#include "arrow/ast/visitor.hpp"

using arrow::ast::Visitor;

Visitor::~Visitor() noexcept { }

void Visitor::run(Node& node) {
  node.accept(*this);
}

#define WARN_UNUSED(fn, type) \
  void Visitor::fn(ast::type& x) { \
    Log::get().error("node %s not implemented for %s", \
      typeid(x).name(), \
      typeid(*this).name() \
    ); \
  }

WARN_UNUSED(visit_node, Node)
WARN_UNUSED(visit_id, Identifier)
WARN_UNUSED(visit_block, Block)
WARN_UNUSED(visit_module, Module)
WARN_UNUSED(visit_extern_function, ExternalFunction)
WARN_UNUSED(visit_function, Function)
WARN_UNUSED(visit_parameter, Parameter)
WARN_UNUSED(visit_call, Call)
WARN_UNUSED(visit_slot, Slot)
WARN_UNUSED(visit_break, Break)
WARN_UNUSED(visit_continue, Continue)
WARN_UNUSED(visit_return, Return)
WARN_UNUSED(visit_int, Integer)
WARN_UNUSED(visit_float, Float)
WARN_UNUSED(visit_bool, Boolean)
WARN_UNUSED(visit_str, String)
WARN_UNUSED(visit_promote, Promote)
WARN_UNUSED(visit_negate_logical, NegateLogical)
WARN_UNUSED(visit_negate_bit, NegateBit)
WARN_UNUSED(visit_negate_numeric, NegateNumeric)
WARN_UNUSED(visit_assign, Assign)
WARN_UNUSED(visit_and, And)
WARN_UNUSED(visit_or, Or)
WARN_UNUSED(visit_eq, EqualTo)
WARN_UNUSED(visit_ne, NotEqualTo)
WARN_UNUSED(visit_lt, LessThan)
WARN_UNUSED(visit_le, LessThanOrEqualTo)
WARN_UNUSED(visit_ge, GreaterThanOrEqualTo)
WARN_UNUSED(visit_gt, GreaterThan)
WARN_UNUSED(visit_bit_and, BitAnd)
WARN_UNUSED(visit_bit_xor, BitXor)
WARN_UNUSED(visit_bit_or, BitOr)
WARN_UNUSED(visit_add, Add)
WARN_UNUSED(visit_sub, Sub)
WARN_UNUSED(visit_mul, Mul)
WARN_UNUSED(visit_div, Div)
WARN_UNUSED(visit_mod, Mod)
WARN_UNUSED(visit_cast, Cast)
WARN_UNUSED(visit_select, Select)
WARN_UNUSED(visit_loop, Loop)
WARN_UNUSED(visit_select_branch, SelectBranch)
WARN_UNUSED(visit_pointer_type, PointerType)
WARN_UNUSED(visit_dereference, Dereference)
WARN_UNUSED(visit_address_of, AddressOf)
WARN_UNUSED(visit_import, Import)
WARN_UNUSED(visit_struct, Structure)
WARN_UNUSED(visit_struct_member, StructureMember)
WARN_UNUSED(visit_member, Member)
