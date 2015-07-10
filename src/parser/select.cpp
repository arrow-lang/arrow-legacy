// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/parser.hpp"

namespace arrow {

// Branch
// -----------------------------------------------------------------------------
// branch = ("if" | "unless") expression block ;
// -----------------------------------------------------------------------------
bool Parser::parse_select_branch() {
  // Expect one-of "if" or "unless"
  auto initial_tok = expect({Token::Type::If, Token::Type::Unless});
  if (!initial_tok) return false;

  // Expect an expression for the condition
  if (!parse_expression()) { return false; }
  auto condition = _stack.front();
  _stack.pop_front();

  // If this is negated; wrap the condition in a `not`
  if (initial_tok->type == Token::Type::Unless) {
    condition = new ast::Not(condition->span, condition);
  }

  // Parse the block
  if (!parse_block(/*top_level=*/false)) { return false; }
  auto block = _stack.front().as<ast::Block>();
  _stack.pop_front();

  // Declare and push the node
  _stack.push_front(new ast::SelectBranch(
    initial_tok->span.extend(block->span),
    condition,
    block
  ));

  return true;
}

// Select
// -----------------------------------------------------------------------------
// TODO
// -----------------------------------------------------------------------------
bool Parser::parse_select() {
  // Declare the node
  Ref<ast::Select> node = new ast::Select(Span(_t.filename(), {0, 0}, {0, 0}));

  // Iterate and attempt to parse as many select branches
  // as we can
  for (unsigned index = 0; ; index += 1) {
    if (index > 0) {
      // Check for "else" followed by a "if" or "unless" to indicate another
      // branch
      if (_t.peek(0)->type == Token::Type::Else && (
          _t.peek(1)->type == Token::Type::If ||
          _t.peek(1)->type == Token::Type::Unless)) {
        // Pop the else
        _t.pop();
      } else {
        // No more branches
        break;
      }
    }

    // Parse branch
    if (!parse_select_branch()) return false;
    auto branch = _stack.front();
    _stack.pop_front();

    // Push branch
    node->branches.push_back(branch);
    if (index == 0) node->span = branch->span;
    else            node->span = node->span.extend(branch->span);

    // Check for "else" followed by a "if" or "unless" to indicate another
    // branch
    if (!(_t.peek(0)->type == Token::Type::Else && (
          _t.peek(1)->type == Token::Type::If ||
          _t.peek(1)->type == Token::Type::Unless))) {
      break;
    }
  }

  // Check for an else block
  if (_t.peek()->type == Token::Type::Else) {
    _t.pop();

    // Parse the block
    if (!parse_block(/*top_level=*/false)) { return false; }
    node->else_block = _stack.front().as<ast::Block>();
    node->span = node->span.extend(node->else_block->span);
    _stack.pop_front();
  }

  _stack.push_front(node);
  return true;
}

}  // namespace arrow
