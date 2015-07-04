// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/parser.hpp"

namespace arrow {

// Parameter
// -----------------------------------------------------------------------------
// parameter = [ "mutable" ] pattern [ ":" type ] [ "=" expression ] ;
// -----------------------------------------------------------------------------
bool Parser::parse_parameter() {
  // Parse a pattern
  auto pattern = expect(&Parser::parse_pattern);
  if (!pattern) return false;

  // Declare the node
  Ref<ast::Parameter> node = new ast::Parameter(
    pattern->span,
    pattern
  );

  // Check for a `:` which would indicate a type annotation
  Ref<ast::Node> annotation = nullptr;
  if (_t.peek()->type == Token::Type::Colon) {
    _t.pop();

    annotation = expect<ast::Type>(&Parser::parse_type);
    if (!annotation) return false;

    node->type = annotation;
    node->span = node->span.extend(annotation->span);
  }

  // Check for an `=` which would indicate a default value
  Ref<ast::Node> val = nullptr;
  if (_t.peek()->type == Token::Type::Equals) {
    _t.pop();

    if (!parse_expression()) return false;
    val = _stack.front();
    _stack.pop_front();

    node->default_value = val;
    node->span = node->span.extend(val->span);
  }

  _stack.push_front(node);

  return true;
}

// Parameters
// -----------------------------------------------------------------------------
// parameters = parameter { "," parameter } [ "," ] ;
// -----------------------------------------------------------------------------
bool Parser::parse_parameters(ast::BaseFunction& x) {
  // Expect a `(`
  if (!expect(Token::Type::LeftParenthesis)) return false;

  // Iterate and parse each parameter in the sequence
  if (!do_sequence(Token::Type::RightParenthesis, [&, this]() {
    // Parse the element
    if (!parse_parameter()) return false;
    x.parameters.push_back(_stack.front());
    _stack.pop_front();

    return true;
  })) return false;

  // Expect a `)`
  auto tok = expect(Token::Type::RightParenthesis);
  if (!tok) return false;
  x.span = x.span.extend(tok->span);

  return true;
}

// Function Result Type
// -----------------------------------------------------------------------------
// function-result-type = "->" type ;
// -----------------------------------------------------------------------------
bool Parser::parse_function_result_type(ast::BaseFunction& x) {
  // Check for a `->` to indicate a result type annotation
  Ref<ast::Node> result_type = nullptr;
  if (_t.peek()->type == Token::Type::Arrow) {
    _t.pop();

    x.result_type = expect<ast::Type>(&Parser::parse_type);
    x.span = x.span.extend(x.result_type->span);
    if (!x.result_type) return false;
  }

  return true;
}

// Function
// -----------------------------------------------------------------------------
// function = [ "export" ] "def" IDENTIFIER "(" [ parameters ] ")" [ "->" type ] block ;
// -----------------------------------------------------------------------------
bool Parser::parse_function() {
  // Check for "export" (to mean that this is a top-level function AND exported)
  Ref<Token> initial_tok = nullptr;
  bool exported = false;
  if (_t.peek()->type == Token::Type::Export) {
    exported = true;
    initial_tok = _t.pop();
  }

  // Expect `def`
  auto tok = expect(Token::Type::Def);
  if (!tok) return false;
  if (!initial_tok) initial_tok = tok;

  // Expect an identifier (for the function name)
  auto id = expect<ast::Identifier>(&Parser::parse_identifier);
  if (!id) return false;
  auto name = id->text;

  // Declare the node
  Ref<ast::Function> node = new ast::Function(
    initial_tok->span,
    exported,
    name
  );

  // Parse the parameter list
  if (!parse_parameters(*node)) return false;

  // Parse the result type annotation (if present)
  if (!parse_function_result_type(*node)) return false;

  // Parse a block
  if (!Parser::parse_block()) return false;
  node->block = _stack.front();
  node->span = node->span.extend(node->block->span);
  _stack.pop_front();

  // Push the node
  _stack.push_front(node);

  return true;
}

// External Function
// -----------------------------------------------------------------------------
// function = [ "export" ] "extern" "def" IDENTIFIER "(" [ parameters ] ")" [ "->" type ] ";" ;
// -----------------------------------------------------------------------------
bool Parser::parse_extern_function() {
  // Check for "export" (to mean that this is a top-level function AND exported)
  Ref<Token> initial_tok = nullptr;
  bool exported = false;
  if (_t.peek()->type == Token::Type::Export) {
    exported = true;
    initial_tok = _t.pop();
  }

  // Expect `extern`
  if (!expect(Token::Type::Extern)) return false;

  // Expect `def`
  auto tok = expect(Token::Type::Def);
  if (!tok) return false;
  if (!initial_tok) initial_tok = tok;

  // Expect an identifier (for the function name)
  auto id = expect<ast::Identifier>(&Parser::parse_identifier);
  if (!id) return false;
  auto name = id->text;

  // Declare the node
  Ref<ast::ExternFunction> node = new ast::ExternFunction(
    initial_tok->span,
    exported,
    name
  );

  // Parse the parameter list
  if (!parse_parameters(*node)) return false;

  // Parse the result type annotation (if present)
  if (!parse_function_result_type(*node)) return false;

  // Expect a `;`
  auto last_tok = expect(Token::Type::Semicolon);
  if (!last_tok) return false;
  node->span = node->span.extend(last_tok->span);

  // Push the node
  _stack.push_front(node);

  return true;
}

}  // namespace arrow
