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
IMPL(Function)
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

Module::Module(Span span)
  : Node(span), sequence() {
}

Unary::Unary(Span span, std::shared_ptr<Node> operand)
  : Node(span), operand(operand) {
}

Return::Return(Span span, std::shared_ptr<Node> expression)
  : Node(span), expression(expression) {
}

Binary::Binary(Span span, std::shared_ptr<Node> lhs, std::shared_ptr<Node> rhs)
  : Node(span), lhs(lhs), rhs(rhs) {
}

Function::Function(Span span, std::shared_ptr<Identifier> name)
  : Node(span), name(name), sequence() {
}

Call::Call(Span span, std::shared_ptr<Node> expression)
  : Node(span), expression(expression) {
}

Slot::Slot(
  Span span,
  std::shared_ptr<Identifier> name,
  std::shared_ptr<Node> type,
  std::shared_ptr<Node> initializer
) : Node(span), name(name), type(type), initializer(initializer) {
}

Integer::Integer(Span span, const std::string& text, unsigned base)
  : TextNode(span, text), base(base) {
}

TextNode::TextNode(Span span, const std::string& text)
  : Node(span), text(text) {
}

Boolean::Boolean(Span span, bool value)
  : Node(span), value(value) {
}

String::String(Span span, const std::vector<std::uint8_t>& bytes)
  : Node(span), bytes(bytes) {
}
