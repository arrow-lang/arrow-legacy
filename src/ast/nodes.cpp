#include "arrow/ast.hpp"

using std::shared_ptr;
using namespace arrow::ast;

#define IMPL(N) \
  N::~N() noexcept { } \
  void N::accept(Visitor& v) { v.visit(*this); }

IMPL(Node)
IMPL(TextNode)
IMPL(Identifier)
IMPL(Module)
IMPL(Break)
IMPL(Return)
IMPL(Integer)
IMPL(Float)
IMPL(Boolean)
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
IMPL(AssignIntDiv)
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
IMPL(IntDiv)

Module::Module()
  : sequence()
{
}

Unary::Unary(std::shared_ptr<Node> operand)
  : operand(operand)
{
}

Return::Return(std::shared_ptr<Node> expression)
  : expression(expression)
{
}

Binary::Binary(std::shared_ptr<Node> lhs, std::shared_ptr<Node> rhs)
  : lhs(lhs), rhs(rhs)
{
}

Integer::Integer(const std::string& text, unsigned base)
  : TextNode(text), base(base)
{
}

TextNode::TextNode(const std::string& text)
  : text(text)
{
}

Boolean::Boolean(bool value)
  : value(value)
{
}