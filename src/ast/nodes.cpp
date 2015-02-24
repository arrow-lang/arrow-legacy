#include "arrow/ast.hpp"

using std::shared_ptr;
using namespace arrow::ast;

#define IMPL(N) \
  N::~N() noexcept { } \
  void N::accept(Visitor& v) { v.visit(*this); }

IMPL(Node)
IMPL(Module)
IMPL(Break)
IMPL(Integer)
IMPL(Float)
IMPL(Boolean)
IMPL(Unary)
IMPL(Promote)
IMPL(NegateNumeric)
IMPL(NegateLogical)
IMPL(NegateBitwise)

Module::Module()
  : sequence()
{
}

Unary::Unary(std::shared_ptr<Node> operand)
  : operand(operand)
{
}

Integer::Integer(const std::string& text, unsigned base)
  : text(text), base(base)
{
}

Float::Float(const std::string& text)
  : text(text)
{
}

Boolean::Boolean(bool value)
  : value(value)
{
}
