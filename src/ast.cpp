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
  auto& node = _tree.add("Module", "");

  _ctx.push(&node);

  for (auto& node : x.sequence) {
    node->accept(*this);
  }

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

void Show::visit(Boolean& x)
{
  _el().add("Boolean", x.value ? "true" : "false");
}

void Show::visit(Promote& x)
{
  auto& node = _el().add("Promote", "");
  _ctx.push(&node);

  x.operand->accept(*this);

  _ctx.pop();
}

void Show::visit(NegateNumeric& x)
{
  auto& node = _el().add("NegateNumeric", "");
  _ctx.push(&node);

  x.operand->accept(*this);

  _ctx.pop();
}

void Show::visit(NegateLogical& x)
{
  auto& node = _el().add("NegateLogical", "");
  _ctx.push(&node);

  x.operand->accept(*this);

  _ctx.pop();
}

void Show::visit(NegateBitwise& x)
{
  auto& node = _el().add("NegateBitwise", "");
  _ctx.push(&node);

  x.operand->accept(*this);

  _ctx.pop();
}
