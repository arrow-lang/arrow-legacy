// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/parser.hpp"

namespace arrow {

// Tuple
// -----------------------------------------------------------------------------
// tuple = "(" ")"
//       | "(" expression "," ")"
//       | "(" expression { "," expression } [ "," ] ")"
//       ;
// -----------------------------------------------------------------------------
bool Parser::parse_tuple() {
  // Expect a `(`
  auto tok = expect(Token::Type::LeftParenthesis);
  if (!tok) return false;

  // Declare the tuple node
  Ref<ast::Tuple> node = new ast::Tuple(tok->span);

  // Check for an immediate `)` which makes this an empty tuple
  if (_t.peek(0)->type == Token::Type::RightParenthesis) {
    tok = _t.pop();
    node->span = node->span.extend(tok->span);
    _stack.push_front(node);
    return true;
  }

  // Parse the first element (which is a special case for tuples)
  if (!parse_expression()) return false;

  // Check for a following comma
  if (_t.peek(0)->type != Token::Type::Comma) {
    // We are not a tuple; return so the inner expression
    // becomes our expression
    // Expect `)`
    if (!expect(Token::Type::RightParenthesis)) { return false; }

    return true;
  } else {
    _t.pop();

    auto elem = _stack.front();
    node->elements.push_back(elem);
    _stack.pop_front();
  }

  // Iterate and parse each remaining element in the sequence
  if (!do_sequence(Token::Type::RightParenthesis, [&, this]() {
    // Parse the element
    if (!parse_expression()) return false;
    node->elements.push_back(_stack.front());
    _stack.pop_front();

    return true;
  })) return false;

  // Expect `)`
  if (!(tok = expect(Token::Type::RightParenthesis))) { return false; }

  // Push the node
  node->span = node->span.extend(tok->span);
  _stack.push_front(node);

  return true;
}

}  // namespace arrow
