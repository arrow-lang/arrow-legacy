// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/parser.hpp"

namespace arrow {

// Integer
// -----------------------------------------------------------------------------
// integer = INTEGER ;
// -----------------------------------------------------------------------------
bool Parser::parse_integer() {
  auto tok = expect<IntegerToken>(Token::Type::Integer);
  if (!tok) return false;

  _stack.push_front(new ast::Integer(tok->span, tok->text));

  return true;
}

// Float
// -----------------------------------------------------------------------------
// float = FLOAT ;
// -----------------------------------------------------------------------------
bool Parser::parse_float() {
  auto tok = expect<FloatToken>(Token::Type::Float);
  if (!tok) return false;

  _stack.push_front(new ast::Float(tok->span, tok->text));

  return true;
}

// Boolean
// -----------------------------------------------------------------------------
// boolean = TRUE | FALSE ;
// -----------------------------------------------------------------------------
bool Parser::parse_boolean() {
  auto tok = expect({Token::Type::True, Token::Type::False});
  if (!tok) return false;

  _stack.push_front(new ast::Boolean(
    tok->span, tok->type == Token::Type::True));

  return true;
}

// String
// -----------------------------------------------------------------------------
// string = STRING ;
// -----------------------------------------------------------------------------
bool Parser::parse_string() {
  auto tok = expect<StringToken>(Token::Type::String);
  if (!tok) return false;

  _stack.push_front(new ast::String(tok->span, tok->text));

  return true;
}

// Identifier
// -----------------------------------------------------------------------------
// identifier = IDENTIFIER ;
// -----------------------------------------------------------------------------
bool Parser::parse_identifier() {
  auto tok = expect<IdentifierToken>(Token::Type::Identifier);
  if (!tok) return false;

  _stack.push_front(new ast::Identifier(tok->span, tok->text));

  return true;
}

}  // namespace arrow
