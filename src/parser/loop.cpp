// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/parser.hpp"

namespace arrow {

// Loop
// -----------------------------------------------------------------------------
// condition-loop = ("while" | "until") expression block ;
// infinite-loop = "loop" block ;
// -----------------------------------------------------------------------------
bool Parser::parse_loop() {
  // Expect one-of "while", "loop", or "until"
  auto initial_tok = expect({
    Token::Type::Loop, Token::Type::While, Token::Type::Until});
  if (!initial_tok) {
    return false;
  }

  // If this is not an infinite-loop; check for a condition
  Ref<ast::Node> condition = nullptr;
  if (initial_tok->type != Token::Type::Loop) {
    if (!parse_expression()) { return false; }
    condition = _stack.front();
    _stack.pop_front();

    // If this is a negated loop; wrap the condition in a `not`
    if (initial_tok->type == Token::Type::Until) {
      condition = new ast::Not(condition->span, condition);
    }
  }

  // Attempt to parse the loop block
  if (!parse_block()) { return false; }
  auto block = _stack.front().as<ast::Block>();
  _stack.pop_front();

  // Declare and push the node
  _stack.push_back(new ast::Loop(
    initial_tok->span.extend(block->span),
    block,
    condition
  ));

  return true;
}

// Break
// -----------------------------------------------------------------------------
// break = "break" ;
// -----------------------------------------------------------------------------
bool Parser::parse_break() {
  auto tok = expect(Token::Type::Break);
  if (!tok) return false;

  _stack.push_back(new ast::Break(tok->span));

  return true;
}

// Continue
// -----------------------------------------------------------------------------
// continue = "continue" ;
// -----------------------------------------------------------------------------
bool Parser::parse_continue() {
  auto tok = expect(Token::Type::Continue);
  if (!tok) return false;

  _stack.push_back(new ast::Continue(tok->span));

  return true;
}

}  // namespace arrow
