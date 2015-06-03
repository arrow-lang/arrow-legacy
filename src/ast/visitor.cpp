// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <boost/property_tree/xml_parser.hpp>

#include "arrow/ast/visitor.hpp"

using arrow::ast::Visitor;

Visitor::~Visitor() noexcept { }

void Visitor::run(Node& node) {
  node.accept(*this);
}

void Visitor::visit_module(Module& x) {
  for (auto& item : x.sequence) {
    item->accept(*this);
  }
}

void Visitor::visit_select(Select& x) {
  for (auto& item : x.branches) {
    item->accept(*this);
  }
}

void Visitor::visit_loop(Loop& x) {
  for (auto& item : x.sequence) {
    item->accept(*this);
  }
}

void Visitor::visit_select_branch(SelectBranch& x) {
  for (auto& item : x.sequence) {
    item->accept(*this);
  }
}
