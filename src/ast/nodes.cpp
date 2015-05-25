// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <gmp.h>

#include <vector>
#include <string>

#include "arrow/ast.hpp"

using std::shared_ptr;
namespace ast = arrow::ast;

#define IMPL(N) \
  ast::N::~N() noexcept { } \
  void ast::N::accept(AbstractVisitor& v) { v.visit(*this); }

IMPL(Node)
IMPL(TextNode)
IMPL(Identifier)
IMPL(Module)
IMPL(AbstractFunction)
IMPL(ExternalFunction)
IMPL(Function)
IMPL(Parameter)
IMPL(Call)
IMPL(Slot)
IMPL(Break)
IMPL(Return)
IMPL(Integer)
IMPL(Float)
IMPL(Boolean)
IMPL(String)
IMPL(Unary)
IMPL(Binary)
IMPL(Promote)
IMPL(NegateNumeric)
IMPL(NegateLogical)
IMPL(NegateBit)
IMPL(Assign)
IMPL(AssignAdd)
IMPL(AssignSub)
IMPL(AssignMul)
IMPL(AssignDiv)
IMPL(AssignMod)
IMPL(AssignBitAnd)
IMPL(AssignBitXor)
IMPL(AssignBitOr)
IMPL(And)
IMPL(Or)
IMPL(EqualTo)
IMPL(NotEqualTo)
IMPL(LessThan)
IMPL(LessThanOrEqualTo)
IMPL(GreaterThanOrEqualTo)
IMPL(GreaterThan)
IMPL(BitAnd)
IMPL(BitXor)
IMPL(BitOr)
IMPL(Add)
IMPL(Sub)
IMPL(Mul)
IMPL(Div)
IMPL(Mod)

ast::Node::Node(Span span)
  : span(span) {
}

ast::Module::Module(Span span)
  : Node(span), sequence() {
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
  std::shared_ptr<Node> initializer
) : Node(span), name(name), type(type), initializer(initializer) {
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
