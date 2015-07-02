// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/parser.hpp"

namespace arrow {

// Pattern
// -----------------------------------------------------------------------------
// pattern-wildcard = "_" ;
// pattern-literal = literal ;
// pattern-identifier = [ "mutable" ] IDENTIFIER ;
// pattern-tuple =
//    | "(" ")"
//    | "(" pattern "," ")"
//    | "(" pattern "," pattern "," pattern [ "," ] ")"
//    ;
// pattern = pattern-wildcard
//         | pattern-literal
//         | pattern-identifier
//         | pattern-tuple ;
// -----------------------------------------------------------------------------
bool Parser::parse_pattern() {
  auto tok = _t.peek(0);

  // Check for a "_" which would indicate a wildcard pattern
  if (tok->type == Token::Type::Underscore) {
    auto tok = _t.pop();

    _stack.push_front(new ast::PatternWildcard(tok->span));
    return true;
  }

  // Check for a `mut` which could optionally preceed an identifier pattern
  bool mut = false;
  if (tok->type == Token::Type::Mutable
      && _t.peek(1)->type == Token::Type::Identifier) {
    _t.pop();
    tok = _t.peek(0);
    mut = true;
  }

  // Check for an IDENTIFIER which would indicate a simple identifier pattern
  if (tok->type == Token::Type::Identifier) {
    auto ident = expect<ast::Identifier>(&Parser::parse_identifier);
    if (!ident) return false;

    _stack.push_front(new ast::PatternIdentifier(tok->span, ident->text, mut));
    return true;
  }

  // Check for a `(` which would be the start of a tuple pattern
  if (tok->type == Token::Type::LeftParenthesis) {
    _t.pop();

    // Declare the tuple-pattern node
    Ref<ast::PatternTuple> node = new ast::PatternTuple(tok->span);

    // Check for an immediate `)` which makes this an empty tuple
    if (_t.peek(0)->type == Token::Type::RightParenthesis) {
      tok = _t.pop();
      node->span = node->span.extend(tok->span);
      _stack.push_front(node);
      return true;
    }

    // Parse the first element (which is a special case for tuples)
    auto elem0 = expect(&Parser::parse_pattern);
    if (!elem0) return false;
    node->elements.push_back(elem0);

    // Expect a comma to follow the first element (to prove we're a tuple)
    if (!expect(Token::Type::Comma)) return false;

    // Iterate and parse each remaining element in the sequence
    if (!do_sequence(Token::Type::RightParenthesis, [&, this]() {
      // Parse the element
      auto elem = expect(&Parser::parse_pattern);
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

  // Check for and parse a literal for a literal pattern
  if (tok->type == Token::Type::Float ||
      tok->type == Token::Type::String ||
      tok->type == Token::Type::Integer ||
      tok->type == Token::Type::True ||
      tok->type == Token::Type::None ||
      tok->type == Token::Type::False) {
    auto literal = expect<ast::Literal>(&Parser::parse_literal);
    if (!literal) return false;

    _stack.push_front(new ast::PatternLiteral(tok->span, literal));
    return true;
  }

  // TODO: Additional patterns

  // Fail; could not match a pattern
  // TODO: Should probably have shorter error message
  expect({
    Token::Type::Mutable,
    Token::Type::Identifier,
    Token::Type::LeftParenthesis,
    Token::Type::Underscore,
    Token::Type::Float,
    Token::Type::True,
    Token::Type::False,
    Token::Type::Integer,
    Token::Type::String,
    Token::Type::None
  });

  return false;
}

}  // namespace arrow
