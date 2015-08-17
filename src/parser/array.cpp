// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/parser.hpp"

namespace arrow {

// Array
// -----------------------------------------------------------------------------
// array = "[" "]"
//       | "[" expression [","] "]"
//       | "[" expression { "," expression } [ "," ] "]"
//       ;
// -----------------------------------------------------------------------------
bool Parser::parse_array() {
  // Expect a `[`
  auto tok = expect(Token::Type::LeftBracket);
  if (!tok) return false;

  // Declare the array node
  Ref<ast::Array> node = new ast::Array(tok->span);

  // Iterate and parse each element in the sequence
  if (!do_sequence(Token::Type::RightBracket, [&, this]() {
    // Parse the element
    if (!parse_expression()) return false;
    node->elements.push_back(_stack.front());
    _stack.pop_front();

    return true;
  })) return false;

  // Expect `]`
  if (!(tok = expect(Token::Type::RightBracket))) { return false; }

  // Push the node
  node->span = node->span.extend(tok->span);
  _stack.push_front(node);

  return true;
}

}  // namespace arrow
