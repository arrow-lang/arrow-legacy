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

Show::Show()
  : _tree(), _el(_tree)
{
}

void Show::show(std::ostream& os)
{
  write_xml(os, _tree);
}

void Show::visit(Module&)
{
  _el.add("Module", "");
}
