// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/parser.hpp"

namespace arrow {

// Structure
// -----------------------------------------------------------------------------
// struct = [ "export" ] "struct" IDENTIFIER "{" [ struct-members ] "}" ;
// struct-members = struct-member { "," struct-member } [ "," ] ;
// struct-member = IDENTIFIER ":" type [ "=" expression ] ;
// -----------------------------------------------------------------------------
bool Parser::parse_struct() {
  // Check for "export"
  Ref<Token> initial_tok = nullptr;
  bool exported = false;
  if (_t.peek()->type == Token::Type::Export) {
    exported = true;
    initial_tok = _t.pop();
  }

  // Expect `struct`
  auto tok = expect(Token::Type::Struct);
  if (!tok) return false;
  if (!initial_tok) initial_tok = tok;

  // Expect an identifier
  auto id = expect<ast::Identifier>(&Parser::parse_identifier);
  if (!id) return false;
  auto name = id->text;

  // Expect `{`
  if (!expect(Token::Type::LeftBrace)) return false;

  // Declare the node
  Ref<ast::Structure> node = new ast::Structure(
    initial_tok->span, exported, name);

  // Iterate and parse each argument in the sequence
  if (!do_sequence(Token::Type::RightBrace, [&, this]() {
    // Expect an identifier
    auto id = expect<ast::Identifier>(&Parser::parse_identifier);
    if (!id) return false;

    // Expect `:`
    if (!expect(Token::Type::Colon)) return false;

    // Parse type annotation
    auto annotation = expect<ast::Type>(&Parser::parse_type);
    auto last_span = annotation->span;
    if (!annotation) return false;

    // Check for an `=` which would indicate a default value
    Ref<ast::Node> val = nullptr;
    if (_t.peek()->type == Token::Type::Equals) {
      _t.pop();

      if (!parse_expression()) return false;
      val = _stack.front();
      last_span = val->span;
      _stack.pop_front();
    }

    // Declare and push the member
    node->members.push_back(new ast::Member(
      id->span.extend(last_span), id->text, annotation, val));

    return true;
  })) return false;

  // Expect `}`
  if (!expect(Token::Type::RightBrace)) return false;

  // Push the node
  _stack.push_front(node);

  return true;
}

}  // namespace arrow
