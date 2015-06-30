// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/ast.hpp"
#include "arrow/ast/show.hpp"

namespace arrow {
namespace ast {

Show::Show() : _s(), _w(_s) {
}

Show::~Show() noexcept {
}

void Show::run(Node& node) {
  // Run the visitor over the given AST node
  Visitor::run(node);

  // Print out the JSON AST
  std::printf("%s\n", _s.GetString());
}

void Show::visit_module(Module& x) {
  _w.StartObject();

  _w.Key("tag");
  _w.String("Module");

  _w.Key("span");
  _w.String(x.span.to_string().c_str());

  _w.Key("name");
  _w.String(x.name.c_str());

  _w.Key("block");
  x.block->accept(*this);

  _w.EndObject();
}

void Show::visit_block(Block& x) {
  _w.StartObject();

  _w.Key("tag");
  _w.String("Block");

  _w.Key("span");
  _w.String(x.span.to_string().c_str());

  _w.Key("statements");
  _w.StartArray();
  for (auto& statement : x.statements) {
    statement->accept(*this);
  }

  _w.EndArray();

  _w.EndObject();
}

}  // namespace ast
}  // namespace arrow
