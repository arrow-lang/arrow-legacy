// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <boost/filesystem.hpp>

#include "arrow/parser.hpp"

namespace fs = boost::filesystem;

namespace arrow {

Parser::Parser(std::shared_ptr<std::istream> is, const std::string& filename)
  : _t(is, filename) {
}

Ref<ast::Node> Parser::parse() {
  // Declare the top-level (root) module
  auto stem = fs::path(_t.filename()).stem().string();
  Ref<ast::Module> mod = new ast::Module(
    Span(_t.filename(), {0, 0}, {0, 0}), stem);

  // Attempt to parse a top-level block
  if (!parse_block(true)) return nullptr;
  mod->block = _stack.front().as<ast::Node>();
  mod->span = mod->span.extend(mod->block->span);
  _stack.pop_front();

  return mod;
}

// Module Statement
// A `statement` that can only be found in the top-level of a module.
// -----------------------------------------------------------------------------
// module-statement = import | struct | extern-function | statement ;
// -----------------------------------------------------------------------------
bool Parser::parse_module_statement() {
  switch (_t.peek()->type) {
    // case Token::Type::Import:
    //   return parse_import();
    //
    // case Token::Type::Struct:
    //   return parse_struct();
    //
    // case Token::Type::Extern:
    //   return parse_extern_function();

    default:
      // We must be a statement
      return parse_statement();
  }
}

// Statement
// -----------------------------------------------------------------------------
// statement = loop
//           | function
//           | slot
//           | select-expression
//           | block-expression
//           | expression-statement
//           ;
// -----------------------------------------------------------------------------
bool Parser::parse_statement() {
  auto tok = _t.peek(0);
  if (tok->type == Token::Type::Export && (
      _t.peek(1)->type == Token::Type::Let ||
      _t.peek(1)->type == Token::Type::Def ||
      _t.peek(1)->type == Token::Type::Extern)) {
    // Ignore the "export" (for now)
    tok = _t.peek(1);
  }

  switch (tok->type) {
    case Token::Type::Loop:
    case Token::Type::While:
    case Token::Type::Until:
      return parse_loop();

    // case Token::Type::Def:
    //   return parse_function();

    case Token::Type::Extern:
      return parse_extern();

    case Token::Type::Let:
      return parse_slot();

    // case Token::Type::If:
    //   return parse_select_expression();

    case Token::Type::LeftBrace:
      return parse_block(/*top_level=*/false);

    case Token::Type::Semicolon:
      // A semicolon as a statement does nothing but does still parse
      _t.pop();
      return true;

    default:
      // We must be an `expression statement`
      return parse_expression_statement();
  }
}

// Expression Statement
// -----------------------------------------------------------------------------
// expression-statement = expression ";" ;
// -----------------------------------------------------------------------------
bool Parser::parse_expression_statement() {
  // Attempt to parse the expression
  if (!parse_expression()) { return false; }

  // Expect `;` to terminate the expression statement
  if (!expect(Token::Type::Semicolon)) return false;

  return true;
}

// Primary Expression
// -----------------------------------------------------------------------------
// primary-expression = INTEGER
//                    | FLOAT
//                    | BOOLEAN
//                    | IDENTIFIER
//                    | STRING
//                    | paren-expression
//                    | select-expression
//                    | block-expression
//                    | break
//                    | continue
//                    | return
//                    ;
// -----------------------------------------------------------------------------
bool Parser::parse_primary_expression() {
  switch (_t.peek()->type) {
    case Token::Type::Integer:
      return parse_integer();

    case Token::Type::Float:
      return parse_float();

    case Token::Type::Identifier:
      return parse_identifier();

    case Token::Type::String:
      return parse_string();

    case Token::Type::True:
    case Token::Type::False:
      return parse_boolean();

    case Token::Type::LeftParenthesis:
      return parse_paren_expression();

    // case Token::Type::If:
    //   return parse_select_expression();
    //
    // case Token::Type::LeftBrace:
    //   return parse_block_expression();

    case Token::Type::Break:
      return parse_break();

    case Token::Type::Continue:
      return parse_continue();

    // case Token::Type::Return:
    //   return parse_return();

    default:
      return false;
  }
}

// Parenthetical Expression
// -----------------------------------------------------------------------------
// paren-expression = "(" expression ")" ;
// -----------------------------------------------------------------------------
bool Parser::parse_paren_expression() {
  // Expect `(`
  if (!expect(Token::Type::LeftParenthesis)) return false;

  // Attempt to parse the inner expression
  if (!parse_expression()) return false;

  // Expect `)`
  if (!expect(Token::Type::RightParenthesis)) return false;

  return true;
}

// Postfix Expression
// -----------------------------------------------------------------------------
// postfix-expression = primary-expression
//                    | call-expression
//                    | member-expression
//                    ;
// -----------------------------------------------------------------------------
bool Parser::parse_postfix_expression() {
  // Parse the operand expression as a primary expression.
  if (!parse_primary_expression()) { return false; }

  // Look for a possible postfix expressions.
  // Continue consumption (left-to-right) until we run out of postfix
  // expressions.
  auto continue_ = true;
  while (continue_) {
    auto tok = _t.peek(0);
    switch (tok->type) {
      case Token::Type::LeftParenthesis:
        if (!parse_call()) { return false; }
        break;

      case Token::Type::Period:
        if (!parse_path()) { return false; }
        break;

      default:
        continue_ = false;
        break;
    }
  }

  return true;
}

// Path Expression
// -----------------------------------------------------------------------------
// path = postfix-expression "." IDENTIFIER ;
// -----------------------------------------------------------------------------
bool Parser::parse_path() {
  // Pull the awaiting operand expression
  auto expr = _stack.front();
  _stack.pop_front();

  // Expect `.`
  if (!expect(Token::Type::Period)) { return false; }

  // Parse the path identifier
  auto id = expect<ast::Identifier>(&Parser::parse_identifier);
  if (!id) return false;

  // Declare and push the node
  _stack.push_back(new ast::Path(expr->span.extend(id->span), expr, id));

  return true;
}

// Return
// -----------------------------------------------------------------------------
// return = "return" expression ;
// -----------------------------------------------------------------------------
bool Parser::parse_return() {
  // Expect `return`
  auto tok = expect(Token::Type::Return);
  if (!tok) { return false; }

  // Check for an early `;` that would indicate no expression to return
  Ref<ast::Node> expr = nullptr;
  if (_t.peek()->type != Token::Type::Semicolon) {
    // Nope; we should attempt to parse the returned expression
    // TODO: Improve expect to handle this case
    if (!parse_expression()) return false;
    auto expr = _stack.front();
    _stack.pop_front();
  }

  // Declare (and push) the node
  _stack.push_front(new ast::Return(
    expr ? tok->span.extend(expr->span) : tok->span,
    expr));

  return true;
}

// External Item
// -----------------------------------------------------------------------------
// extern = extern-slot | extern-function ;
// -----------------------------------------------------------------------------
bool Parser::parse_extern() {
  auto pindex = 0;
  if (_t.peek(pindex)->type == Token::Type::Export) {
    // Ignore the "export" (for now)
    pindex += 1;
  }

  if (_t.peek(pindex)->type != Token::Type::Extern) {
    expect(Token::Type::Extern);
    return false;
  } else {
    pindex += 1;
  }

  if (_t.peek(pindex)->type == Token::Type::Let) {
    return parse_extern_slot();
  }

  return parse_extern_function();
}

}  // namespace arrow
