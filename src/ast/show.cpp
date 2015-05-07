// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <boost/property_tree/xml_parser.hpp>

#include "arrow/ast/show.hpp"

using arrow::ast::Show;

Show::Show(std::ostream& os)
  : _stream{os}, _tree{}, _ctx{} {
}

Show::~Show() noexcept { }

void Show::run(Node& node) {
  AbstractVisitor::run(node);

  // Write out the result
  boost::property_tree::xml_writer_settings<char> settings(' ', 1);
  write_xml(_stream, _tree, settings);
}

boost::property_tree::ptree& Show::_el() {
  if (_ctx.size() == 0) {
    return _tree;
  } else {
    return *(_ctx.top());
  }
}

void Show::visit(Module& x) {
  auto& node = _el().add("Module", "");
  _ctx.push(&node);

  for (auto& item : x.sequence) {
    item->accept(*this);
  }

  _ctx.pop();
}

void Show::visit(Return& x) {
  auto& node = _el().add("Return", "");
  _ctx.push(&node);

  x.expression->accept(*this);

  _ctx.pop();
}

void Show::visit(Break&) {
  _el().add("Break", "");
}

void Show::visit(Integer& x) {
  auto& node = _el().add("Integer", x.text.c_str());
  node.add("<xmlattr>.base", std::to_string(x.base).c_str());
}

void Show::visit(Float& x) {
  _el().add("Float", x.text.c_str());
}

void Show::visit(Identifier& x) {
  _el().add("Identifier", x.text.c_str());
}

void Show::visit(Boolean& x) {
  _el().add("Boolean", x.value ? "true" : "false");
}

void Show::visit(String& x) {
  _el().add("String", x.text());
}

void Show::handle_unary(const std::string& name, Unary& x) {
  auto& node = _el().add(name.c_str(), "");
  _ctx.push(&node);

  x.operand->accept(*this);

  _ctx.pop();
}

#define SHOW_UNARY(N) \
  void Show::visit(N& x) { handle_unary(#N, x); }

SHOW_UNARY(Promote)
SHOW_UNARY(NegateNumeric)
SHOW_UNARY(NegateLogical)
SHOW_UNARY(NegateBit)

void Show::handle_binary(const std::string& name, Binary& x) {
  auto& node = _el().add(name.c_str(), "");
  _ctx.push(&node);

  x.lhs->accept(*this);
  x.rhs->accept(*this);

  _ctx.pop();
}

#define SHOW_BINARY(N) \
  void Show::visit(N& x) { handle_binary(#N, x); }

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

void Show::visit(Function& x) {
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

void Show::visit(Call& x) {
  auto& item = _el().add("Call", "");
  _ctx.push(&item);

  x.expression->accept(*this);

  _ctx.pop();
}

void Show::visit(Slot& x) {
  auto& item = _el().add("Slot", "");
  _ctx.push(&item);

  item.add("<xmlattr>.name", x.name->text.c_str());

  if (x.type != nullptr) {
    auto& type = _el().add("Type", "");
    _ctx.push(&type);

    x.type->accept(*this);

    _ctx.pop();
  }

  if (x.initializer != nullptr) {
    auto& init = _el().add("Initializer", "");
    _ctx.push(&init);

    x.initializer->accept(*this);

    _ctx.pop();
  }

  _ctx.pop();
}
