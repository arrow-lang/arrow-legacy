// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/ast.hpp"

#define IMPL(N) \
  N::~N() noexcept { }

#define IMPL_ACCEPT(N, T) \
  IMPL(N) \
  void N::accept(Visitor& v) { v.visit_##T(*this); }

namespace arrow {
namespace ast {

IMPL_ACCEPT(Node, node)
IMPL(Item)
IMPL(Unary)
IMPL(Binary)

IMPL_ACCEPT(Module, module)
IMPL_ACCEPT(Identifier, id)
IMPL_ACCEPT(Block, block)
IMPL_ACCEPT(Import, import)

IMPL(Literal)
IMPL_ACCEPT(Boolean, bool)
IMPL_ACCEPT(String, str)
IMPL_ACCEPT(Float, float)
IMPL_ACCEPT(Integer, int)
IMPL_ACCEPT(None, none)

IMPL_ACCEPT(Loop, loop)
IMPL_ACCEPT(Break, break)
IMPL_ACCEPT(Continue, continue)

IMPL_ACCEPT(Identity, identity)
IMPL_ACCEPT(BitNot, bit_not)
IMPL_ACCEPT(Negate, negate)
IMPL_ACCEPT(Dereference, deref)
IMPL_ACCEPT(Not, not)
IMPL_ACCEPT(AddressOf, address_of)

IMPL_ACCEPT(Add, add)
IMPL_ACCEPT(Sub, sub)
IMPL_ACCEPT(Mul, mul)
IMPL_ACCEPT(Div, div)
IMPL_ACCEPT(Mod, mod)

IMPL_ACCEPT(BitAnd, bit_and)
IMPL_ACCEPT(BitXor, bit_xor)
IMPL_ACCEPT(BitOr, bit_or)

IMPL_ACCEPT(EqualTo, eq)
IMPL_ACCEPT(NotEqualTo, ne)
IMPL_ACCEPT(GreaterThanOrEqualTo, ge)
IMPL_ACCEPT(GreaterThan, gt)
IMPL_ACCEPT(LessThanOrEqualTo, le)
IMPL_ACCEPT(LessThan, lt)

IMPL_ACCEPT(And, and)
IMPL_ACCEPT(Or, or)

IMPL_ACCEPT(Assign, assign)

IMPL_ACCEPT(Cast, cast)

IMPL_ACCEPT(Path, path)

IMPL_ACCEPT(Call, call)
IMPL_ACCEPT(Argument, argument)

IMPL(BaseFunction)
IMPL_ACCEPT(Function, function)
IMPL_ACCEPT(ExternFunction, extern_function)
IMPL_ACCEPT(Return, return)

IMPL_ACCEPT(Slot, slot)
IMPL_ACCEPT(ExternSlot, extern_slot)

IMPL(Pattern)
IMPL_ACCEPT(PatternWildcard, pattern_wildcard)
IMPL_ACCEPT(PatternIdentifier, pattern_identifier)
IMPL_ACCEPT(PatternLiteral, pattern_literal)
IMPL_ACCEPT(PatternTuple, pattern_tuple)

IMPL(Type)
IMPL_ACCEPT(TypeNone, type_none)
IMPL_ACCEPT(TypeTuple, type_tuple)
IMPL_ACCEPT(TypePath, type_path)
IMPL_ACCEPT(TypePointer, type_pointer)

IMPL_ACCEPT(Tuple, tuple)

}  // namespace ast
}  // namespace arrow
