// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <boost/property_tree/xml_parser.hpp>

#include <string>

#include "arrow/ast/show.hpp"

using arrow::ast::Show;

Show::Show(std::ostream& os)
  : _stream(os), _tree(), _ctx() {
}

Show::~Show() noexcept { }

void Show::run(Node& node) {
  Visitor::run(node);

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

void Show::visit_module(Module& x) {
  auto& node = _el().add("Module", "");
  _ctx.push(&node);

  for (auto& item : x.sequence) {
    item->accept(*this);
  }

  _ctx.pop();
}

void Show::visit_select(Select& x) {
  auto& node = _el().add("Select", "");
  _ctx.push(&node);

  for (auto& item : x.branches) {
    item->accept(*this);
  }

  _ctx.pop();
}

void Show::visit_select_branch(SelectBranch& x) {
  auto& node = _el().add("SelectBranch", "");
  _ctx.push(&node);

  if (x.condition) {
    auto& cond = _el().add("Condition", "");
    _ctx.push(&cond);

    x.condition->accept(*this);

    _ctx.pop();
  }

  auto& seq = _el().add("Sequence", "");
  _ctx.push(&seq);

  for (auto& item : x.sequence) {
    item->accept(*this);
  }

  _ctx.pop();

  _ctx.pop();
}

void Show::visit_return(Return& x) {
  auto& node = _el().add("Return", "");
  if (x.expression) {
    _ctx.push(&node);

    x.expression->accept(*this);

    _ctx.pop();
  }
}

void Show::visit_break(Break&) {
  _el().add("Break", "");
}

void Show::visit_int(Integer& x) {
  auto& node = _el().add("Integer", x.text.c_str());
  node.add("<xmlattr>.base", std::to_string(x.base).c_str());
}

void Show::visit_float(Float& x) {
  _el().add("Float", x.text.c_str());
}

void Show::visit_id(Identifier& x) {
  _el().add("Identifier", x.text.c_str());
}

void Show::visit_bool(Boolean& x) {
  _el().add("Boolean", x.value ? "true" : "false");
}

void Show::visit_str(String& x) {
  _el().add("String", x.text());
}

void Show::handle_unary(const std::string& name, Unary& x) {
  auto& node = _el().add(name.c_str(), "");
  _ctx.push(&node);

  x.operand->accept(*this);

  _ctx.pop();
}

#define SHOW_UNARY(N, D) \
  void Show::visit_##D(N& x) { handle_unary(#N, x); }

SHOW_UNARY(Promote, promote)
SHOW_UNARY(NegateNumeric, negate_numeric)
SHOW_UNARY(NegateLogical, negate_logical)
SHOW_UNARY(NegateBit, negate_bit)
SHOW_UNARY(Dereference, dereference)

void Show::handle_binary(const std::string& name, Binary& x) {
  auto& node = _el().add(name.c_str(), "");
  _ctx.push(&node);

  x.lhs->accept(*this);
  x.rhs->accept(*this);

  _ctx.pop();
}

#define SHOW_BINARY(N, D) \
  void Show::visit_##D(N& x) { handle_binary(#N, x); }

SHOW_BINARY(Assign, assign)
SHOW_BINARY(And, and)
SHOW_BINARY(Or, or)
SHOW_BINARY(EqualTo, eq)
SHOW_BINARY(NotEqualTo, ne)
SHOW_BINARY(LessThan, lt)
SHOW_BINARY(LessThanOrEqualTo, le)
SHOW_BINARY(GreaterThanOrEqualTo, ge)
SHOW_BINARY(GreaterThan, gt)
SHOW_BINARY(BitAnd, bit_and)
SHOW_BINARY(BitXor, bit_xor)
SHOW_BINARY(BitOr, bit_or)
SHOW_BINARY(Add, add)
SHOW_BINARY(Sub, sub)
SHOW_BINARY(Mul, mul)
SHOW_BINARY(Div, div)
SHOW_BINARY(Mod, mod)

void Show::visit_function(Function& x) {
  auto& fn = _el().add("Function", "");
  _ctx.push(&fn);

  fn.add("<xmlattr>.name", x.name->text.c_str());

  if (x.result != nullptr) {
    auto& type = _el().add("Result", "");
    _ctx.push(&type);

    x.result->accept(*this);

    _ctx.pop();
  }

  auto& params = _el().add("Parameters", "");
  _ctx.push(&params);

  for (auto& item : x.parameters) {
    item->accept(*this);
  }

  _ctx.pop();

  auto& seq = _el().add("Sequence", "");
  _ctx.push(&seq);

  for (auto& item : x.sequence) {
    item->accept(*this);
  }

  _ctx.pop();

  _ctx.pop();
}

void Show::visit_extern_function(ExternalFunction& x) {
  auto& fn = _el().add("ExternalFunction", "");
  _ctx.push(&fn);

  fn.add("<xmlattr>.name", x.name->text.c_str());

  if (x.result != nullptr) {
    auto& type = _el().add("Result", "");
    _ctx.push(&type);

    x.result->accept(*this);

    _ctx.pop();
  }

  auto& params = _el().add("Parameters", "");
  _ctx.push(&params);

  for (auto& item : x.parameters) {
    item->accept(*this);
  }

  _ctx.pop();

  _ctx.pop();
}

void Show::visit_call(Call& x) {
  auto& item = _el().add("Call", "");
  _ctx.push(&item);

  auto& expr = _el().add("Expression", "");
  _ctx.push(&expr);
  x.expression->accept(*this);
  _ctx.pop();

  auto& args = _el().add("Arguments", "");
  _ctx.push(&args);

  for (auto& item : x.arguments) {
    item->accept(*this);
  }

  _ctx.pop();

  _ctx.pop();
}

void Show::visit_slot(Slot& x) {
  auto& item = _el().add("Slot", "");
  _ctx.push(&item);

  item.add("<xmlattr>.name", x.name->text.c_str());
  item.add("<xmlattr>.mutable", x.is_mutable);

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

void Show::visit_parameter(Parameter& x) {
  auto& item = _el().add("Parameter", "");
  _ctx.push(&item);

  item.add("<xmlattr>.name", x.name->text.c_str());

  auto& type = _el().add("Type", "");
  _ctx.push(&type);

  x.type->accept(*this);

  _ctx.pop();

  _ctx.pop();
}

void Show::visit_loop(Loop& x) {
  auto& node = _el().add("Loop", "");
  _ctx.push(&node);

  if (x.condition) {
    auto& cond = _el().add("Condition", "");
    _ctx.push(&cond);

    x.condition->accept(*this);

    _ctx.pop();
  }

  auto& seq = _el().add("Sequence", "");
  _ctx.push(&seq);

  for (auto& item : x.sequence) {
    item->accept(*this);
  }

  _ctx.pop();

  _ctx.pop();
}

void Show::visit_pointer_type(PointerType& x) {
  auto& item = _el().add("PointerType", "");
  _ctx.push(&item);

  item.add("<xmlattr>.mutable", x.is_mutable);

  x.pointee->accept(*this);

  _ctx.pop();
}

void Show::visit_address_of(AddressOf& x) {
  auto& node = _el().add("AddressOf", "");
  _ctx.push(&node);

  node.add("<xmlattr>.mutable", x.is_mutable);

  x.operand->accept(*this);

  _ctx.pop();
}
