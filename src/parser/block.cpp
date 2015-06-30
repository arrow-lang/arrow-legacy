// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/parser.hpp"

namespace arrow {

// Block
// -----------------------------------------------------------------------------
// block = "{" { statement } "}" ;
// -----------------------------------------------------------------------------
bool Parser::parse_block(bool top_level) {
  // Expect `{` (for not a top-level block)
  auto begin = Position(0, 0);
  if (!top_level) {
    auto possible_begin = expect(Token::Type::LeftBrace);
    if (!possible_begin) return false;
    else {
      begin = possible_begin->span.begin;
    }
  }

  // Declare the node
  Ref<ast::Block> node = new ast::Block(Span(_t.filename(), begin, begin));

  // Enumerate and attempt to match rules until the token stream
  // is empty
  while (_t.peek()->type != Token::Type::End) {
    if (!top_level && _t.peek()->type == Token::Type::RightBrace) {
      break;
    }

    // Remember the initial size
    auto initial_size = _stack.size();

    // Parse a statement or module-statement (depending on if
    // we're a top-level block or not)
    auto rv = false;
    if (top_level) rv = parse_module_statement();
    else           rv = parse_statement();

    // Check to see if we received a statement
    // NOTE: There are cases where we can succeed in parsing and
    //       have nothing on the stack (see: `;;;`).
    if (rv && initial_size < _stack.size()) {
      // Consume the stack
      auto stmt = _stack.front();
      node->statements.push_back(stmt);

      // Update the module span (location tracking)
      node->span = node->span.extend(stmt->span);
    }

    // Clear the stack (in case of failure)
    _stack.clear();
  }

  // Expect `}` (for not a top-level block)
  if (!top_level) {
    auto possible_end = expect(Token::Type::RightBrace);
    if (!possible_end) return false;
    else {
      node->span.end = possible_end->span.end;
    }
  }

  // Push the node
  _stack.push_back(node);

  return true;
}

}  // namespace arrow
