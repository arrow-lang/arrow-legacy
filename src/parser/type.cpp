// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/parser.hpp"

namespace arrow {

// Type
// -----------------------------------------------------------------------------
// type = identifier
//      | "None"
//      | "*" type
//      | "(" ")"
//      | "(" type "," ")"
//      | "(" type "," type { "," type } [ "," ] ")"
//      | function-type
//      ;
// function-type-parameter = [ "identifier" ":" ] type ;
// function-type = "(" ")" "->" type
//               | "(" function-type-parameter { "," function-type-parameter } [ "," ] ")" "->" type
//               ;
// -----------------------------------------------------------------------------
bool Parser::parse_type() {
  auto tok = _t.peek(0);

  // Check for `None` to indicate the unit type
  if (tok->type == Token::Type::None) {
    auto tok = _t.pop();

    _stack.push_front(new ast::TypeNone(tok->span));
    return true;
  }

  // Check for an identifier (for a simple type)
  if (tok->type == Token::Type::Identifier) {
    std::vector<std::string> segments;
    auto span = tok->span;

    for (unsigned i = 0; ; i++) {
      // Expect a `.` (unless this is the first identifier)
      if (i > 0 && !expect(Token::Type::Period)) return false;

      // Parse the identifier
      auto id_node = expect<ast::Identifier>(&Parser::parse_identifier);
      if (!id_node) return false;

      segments.push_back(id_node->text);

      // Extend the span
      span = span.extend(id_node->span);

      // Check for a `.` that could continue this into a path
      if (_t.peek(0)->type != Token::Type::Period) break;
    }

    _stack.push_front(new ast::TypePath(
      span,
      segments
    ));

    return true;
  }

  // Check for a `*` to indicate a pointer type
  if (tok->type == Token::Type::Asterisk) {
    _t.pop();

    // Check for `mutable` to indicate a mutable pointee
    bool is_mutable = false;
    if (_t.peek(0)->type == Token::Type::Mutable) {
      _t.pop();
      is_mutable = true;
    }

    // Parse the pointee type
    auto pointee = expect(&Parser::parse_type);
    if (!pointee) return false;

    _stack.push_front(new ast::TypePointer(
      tok->span.extend(pointee->span),
      pointee,
      is_mutable
    ));

    return true;
  }

  // Check for a `(` which would be the start of a tuple type
  if (tok->type == Token::Type::LeftParenthesis) {
    _t.pop();

    // Declare the tuple-type node
    Ref<ast::TypeTuple> node = new ast::TypeTuple(tok->span);

    // Check for an immediate `)` which makes this an empty tuple
    if (_t.peek(0)->type == Token::Type::RightParenthesis) {
      tok = _t.pop();
      node->span = node->span.extend(tok->span);
      _stack.push_front(node);
      return true;
    }

    // Parse the first element (which is a special case for tuples)
    auto elem0 = expect(&Parser::parse_type);
    if (!elem0) return false;
    node->elements.push_back(elem0);

    // Expect a comma to follow the first element (to prove we're a tuple)
    if (!expect(Token::Type::Comma)) return false;

    // Iterate and parse each remaining element in the sequence
    if (!do_sequence(Token::Type::RightParenthesis, [&, this]() {
      // Parse the element
      auto elem = expect(&Parser::parse_type);
      if (!elem) return false;

      // Push the element
      node->elements.push_back(elem);

      return true;
    })) return false;

    // Expect `)`
    if (!(tok = expect(Token::Type::RightParenthesis))) { return false; }

    // Push the node
    node->span = node->span.extend(tok->span);
    _stack.push_front(node);

    return true;
  }

  // TODO: type(X)

  // Fail; could not match a pattern
  // TODO: Should probably have shorter error message
  expect({
    Token::Type::Identifier,
    Token::Type::LeftParenthesis,
    Token::Type::Asterisk,
    Token::Type::None
  });

  return false;
}

}  // namespace arrow
