#include <iostream>
#include <boost/property_tree/xml_parser.hpp>
#include "arrow/ast.hpp"

using boost::property_tree::write_xml;
using boost::property_tree::xml_writer_settings;

using std::shared_ptr;
using namespace arrow::ast;

#define IMPL(N) \
  void N::accept(Visitor& v) { v.visit(*this); }

IMPL(Node);
IMPL(Module);
IMPL(Break);

Show::Show()
  : _tree()
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
