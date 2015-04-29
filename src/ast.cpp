#include <iostream>
#include <boost/property_tree/xml_parser.hpp>
#include "arrow/ast.hpp"

using boost::property_tree::write_xml;
using boost::property_tree::xml_writer_settings;

using std::shared_ptr;
using namespace arrow::ast;

Visitor::~Visitor()
{
}

Show::~Show()
{
}

Show::Show()
  : _tree(), _ctx()
{
}

void Show::show(std::ostream& os)
{
  boost::property_tree::xml_writer_settings<char> settings(' ', 1);
  write_xml(os, _tree, settings);
}

boost::property_tree::ptree& Show::_el()
{
  if (_ctx.size() == 0) {
    return _tree;
  } else {
    return *(_ctx.top());
  }
}

void Show::visit(Module& x)
{
  auto& node = _el().add("Module", "");

  _ctx.push(&node);

  for (auto& item : x.sequence) {
    item->accept(*this);
  }

  _ctx.pop();
}

void Show::visit(Return& x)
{
  auto& node = _el().add("Return", "");

  _ctx.push(&node);

  x.expression->accept(*this);

  _ctx.pop();
}

void Show::visit(Break&)
{
  _el().add("Break", "");
}

void Show::visit(Integer& x)
{
  auto& node = _el().add("Integer", x.text.c_str());
  node.add("<xmlattr>.base", std::to_string(x.base).c_str());
}

void Show::visit(Float& x)
{
  _el().add("Float", x.text.c_str());
}

void Show::visit(Identifier& x)
{
  _el().add("Identifier", x.text.c_str());
}

void Show::visit(Boolean& x)
{
  _el().add("Boolean", x.value ? "true" : "false");
}

void Show::visit(String& x)
{
  _el().add("String", x.text());
}

void Show::visit_unary(const std::string& name, Unary& x)
{
  auto& node = _el().add(name.c_str(), "");
  _ctx.push(&node);

  x.operand->accept(*this);

  _ctx.pop();
}

#define SHOW_UNARY(N) \
  void Show::visit(N& x) { visit_unary(#N, x); }

SHOW_UNARY(Promote)
SHOW_UNARY(NegateNumeric)
SHOW_UNARY(NegateLogical)
SHOW_UNARY(NegateBit)

void Show::visit_binary(const std::string& name, Binary& x)
{
  auto& node = _el().add(name.c_str(), "");
  _ctx.push(&node);

  x.lhs->accept(*this);
  x.rhs->accept(*this);

  _ctx.pop();
}

#define SHOW_BINARY(N) \
  void Show::visit(N& x) { visit_binary(#N, x); }

SHOW_BINARY(Assign)
SHOW_BINARY(AssignAdd)
SHOW_BINARY(AssignSub)
SHOW_BINARY(AssignMul)
SHOW_BINARY(AssignDiv)
SHOW_BINARY(AssignMod)
SHOW_BINARY(AssignBitAnd)
SHOW_BINARY(AssignBitXor)
SHOW_BINARY(AssignBitOr)
SHOW_BINARY(And)
SHOW_BINARY(Or)
SHOW_BINARY(EqualTo)
SHOW_BINARY(NotEqualTo)
SHOW_BINARY(LessThan)
SHOW_BINARY(LessThanOrEqualTo)
SHOW_BINARY(GreaterThanOrEqualTo)
SHOW_BINARY(GreaterThan)
SHOW_BINARY(BitAnd)
SHOW_BINARY(BitXor)
SHOW_BINARY(BitOr)
SHOW_BINARY(Add)
SHOW_BINARY(Sub)
SHOW_BINARY(Mul)
SHOW_BINARY(Div)
SHOW_BINARY(Mod)

void Show::visit(Function& x)
{
  auto& fn = _el().add("Function", "");
  _ctx.push(&fn);

  fn.add("<xmlattr>.name", x.name->text.c_str());

  auto& seq = _el().add("Sequence", "");
  _ctx.push(&seq);

  for (auto& item : x.sequence) {
    item->accept(*this);
  }

  _ctx.pop();
  _ctx.pop();
}

void Show::visit(Call& x)
{
  auto& item = _el().add("Call", "");
  _ctx.push(&item);

  x.expression->accept(*this);

  _ctx.pop();
}
