// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/parser.hpp"

namespace arrow {

// Slot
// -----------------------------------------------------------------------------
// slot = [ "export" ] "let" pattern [ ":" type ] [ "=" expression ] ";" ;
// -----------------------------------------------------------------------------
bool Parser::parse_slot() {
  // Check for "export" (to mean that this is a top-level slot AND exported)
  Ref<Token> initial_tok = nullptr;
  bool exported = false;
  if (_t.peek()->type == Token::Type::Export) {
    exported = true;
    initial_tok = _t.pop();
  }

  // Expect `let`
  auto tok = expect(Token::Type::Let);
  if (!tok) return false;
  if (!initial_tok) initial_tok = tok;

  // Expect a (binding) pattern
  auto pattern = expect<ast::Pattern>(&Parser::parse_pattern);
  if (!pattern) return false;

  // Check for a `:` which would indicate a type annotation
  Ref<ast::Node> annotation = nullptr;
  if (_t.peek()->type == Token::Type::Colon) {
    _t.pop();

    annotation = expect<ast::Type>(&Parser::parse_type);
    if (!annotation) return false;
  }

  // Check for an `=` which would indicate an initializer
  Ref<ast::Node> initializer = nullptr;
  if (_t.peek()->type == Token::Type::Equals) {
    _t.pop();

    if (!parse_expression()) return false;
    initializer = _stack.front();
    _stack.pop_front();
  }

  // Expect `;`
  auto last_tok = expect(Token::Type::Semicolon);
  if (!last_tok) return false;

  // Declare and push the node
  _stack.push_front(new ast::Slot(
    initial_tok->span.extend(last_tok->span),
    exported,
    pattern,
    annotation,
    initializer
  ));

  return true;
}

// External Slot
// -----------------------------------------------------------------------------
// extern-slot = [ "export" ] "extern" "let" IDENTIFIER ":" type ";" ;
// -----------------------------------------------------------------------------
bool Parser::parse_extern_slot() {
  // Check for "export" (to mean that this is a top-level slot AND exported)
  Ref<Token> initial_tok = nullptr;
  bool exported = false;
  if (_t.peek()->type == Token::Type::Export) {
    exported = true;
    initial_tok = _t.pop();
  }

  // Expect `extern`
  auto tok = expect(Token::Type::Extern);
  if (!tok) return false;
  if (!initial_tok) initial_tok = tok;

  // Expect `let`
  if (!expect(Token::Type::Let)) return false;

  // Expect an identifier
  auto id = expect<ast::Identifier>(&Parser::parse_identifier);
  if (!id) return false;
  auto name = id->text;

  // Expect `:` (which would indicate a type annotation)
  // NOTE: External slots must have a type annotation
  if (!expect(Token::Type::Colon)) return false;

  // Parse the type annotation
  auto annotation = expect<ast::Type>(&Parser::parse_type);
  if (!annotation) return false;

  // Expect `;`
  auto last_tok = expect(Token::Type::Semicolon);
  if (!last_tok) return false;

  // Declare and push the node
  _stack.push_front(new ast::ExternSlot(
    initial_tok->span.extend(last_tok->span),
    exported,
    name,
    annotation
  ));

  return true;
}

}  // namespace arrow
