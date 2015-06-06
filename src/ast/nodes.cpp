// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <cstddef>
#include <cstdlib>
#include <gmp.h>

#include <vector>
#include <string>

#include "arrow/ast.hpp"

using std::shared_ptr;
namespace ast = arrow::ast;

#define IMPL(N, D) \
  ast::N::~N() noexcept { } \
  void ast::N::accept(Visitor& v) { v.visit_##D(*this); }

#define IMPL_ABSTRACT(N) \
  ast::N::~N() noexcept { }

IMPL(Node, node)
IMPL_ABSTRACT(TextNode)
IMPL_ABSTRACT(AbstractFunction)
IMPL_ABSTRACT(Binary)
IMPL_ABSTRACT(Unary)

IMPL(Block, block)
IMPL(Identifier, id)
IMPL(Module, module)
IMPL(ExternalFunction, extern_function)
IMPL(Function, function)
IMPL(Parameter, parameter)
IMPL(Call, call)
IMPL(Slot, slot)
IMPL(Break, break)
IMPL(Return, return)
IMPL(Integer, int)
IMPL(Float, float)
IMPL(Boolean, bool)
IMPL(String, str)
IMPL(Promote, promote)
IMPL(NegateNumeric, negate_numeric)
IMPL(NegateLogical, negate_logical)
IMPL(NegateBit, negate_bit)
IMPL(Assign, assign)
IMPL(And, and)
IMPL(Or, or)
IMPL(EqualTo, eq)
IMPL(NotEqualTo, ne)
IMPL(LessThan, lt)
IMPL(LessThanOrEqualTo, le)
IMPL(GreaterThanOrEqualTo, ge)
IMPL(GreaterThan, gt)
IMPL(BitAnd, bit_and)
IMPL(BitXor, bit_xor)
IMPL(BitOr, bit_or)
IMPL(Add, add)
IMPL(Sub, sub)
IMPL(Mul, mul)
IMPL(Div, div)
IMPL(Mod, mod)
IMPL(Cast, cast)
IMPL(Select, select)
IMPL(SelectBranch, select_branch)
IMPL(Loop, loop)
IMPL(PointerType, pointer_type)
IMPL(AddressOf, address_of)
IMPL(Dereference, dereference)

ast::Node::Node(Span span)
  : span(span) {
}

ast::Module::Module(Span span, const std::string& name)
  : Node(span), sequence(), name(name) {
}

ast::Unary::Unary(Span span, std::shared_ptr<Node> operand)
  : Node(span), operand(operand) {
}

ast::Return::Return(Span span, std::shared_ptr<Node> expression)
  : Node(span), expression(expression) {
}

ast::Binary::Binary(
  Span span, std::shared_ptr<Node> lhs, std::shared_ptr<Node> rhs
)
  : Node(span), lhs(lhs), rhs(rhs) {
}

ast::AbstractFunction::AbstractFunction(
  Span span,
  std::shared_ptr<Identifier> name,
  std::shared_ptr<Node> result
)
  : Node(span), name(name), result(result), parameters() {
}

ast::Call::Call(Span span, std::shared_ptr<Node> expression)
  : Node(span), expression(expression) {
}

ast::Parameter::Parameter(
  Span span,
  std::shared_ptr<Identifier> name,
  std::shared_ptr<Node> type
) : Node(span), name(name), type(type) {
}

ast::Slot::Slot(
  Span span,
  std::shared_ptr<Identifier> name,
  std::shared_ptr<Node> type,
  std::shared_ptr<Node> initializer,
  bool is_mutable
) : Node(span), name(name), type(type), initializer(initializer),
    is_mutable(is_mutable) {
}

ast::Integer::Integer(Span span, const std::string& text, unsigned base)
  : TextNode(span, text), base(base) {
}

ast::TextNode::TextNode(Span span, const std::string& text)
  : Node(span), text(text) {
}

ast::Boolean::Boolean(Span span, bool value)
  : Node(span), value(value) {
}

ast::String::String(Span span, const std::vector<std::uint8_t>& bytes)
  : Node(span), bytes(bytes) {
}

std::uint64_t ast::Integer::minimum_bits() const {
  // Find the number of bits we need (at least) to store
  // this integer value
  mpz_t value;
  mpz_init_set_str(value, text.c_str(), base);
  auto size = mpz_sizeinbase(value, 2);
  mpz_clear(value);
  return size;
}

ast::SelectBranch::SelectBranch(
  Span span,
  std::shared_ptr<Node> condition
) : Block(span), condition(condition) {
}

ast::Loop::Loop(
  Span span,
  std::shared_ptr<Node> condition
) : Block(span), condition(condition) {
}

ast::PointerType::PointerType(
  Span span,
  std::shared_ptr<Node> pointee,
  bool _mutable
) : Node(span), pointee(pointee), is_mutable(_mutable) {
}

ast::AddressOf::AddressOf(
  Span span,
  std::shared_ptr<Node> operand,
  bool _mutable
) : Unary(span, operand), is_mutable(_mutable) {
}
