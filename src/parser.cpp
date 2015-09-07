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
  auto tok = _t.peek(0);
  if (tok->type == Token::Type::Export && (
      (_t.peek(1)->type == Token::Type::Extern ||
       _t.peek(1)->type == Token::Type::Use ||
       _t.peek(1)->type == Token::Type::Struct))) {
    // Ignore the "export" (for now)
    tok = _t.peek(1);
  }

  switch (tok->type) {
    case Token::Type::Import:
      return parse_import();

    case Token::Type::Struct:
      return parse_struct();

    case Token::Type::Use:
      return parse_alias();

    case Token::Type::Extern:
      return parse_extern();

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

    case Token::Type::Let:
      return parse_slot();

    case Token::Type::Def:
      return parse_function();

    case Token::Type::If:
    case Token::Type::Unless:
      return parse_select();

    case Token::Type::LeftBrace:
      return parse_block(/*top_level=*/false);

    case Token::Type::Semicolon:
      // A semicolon as a statement does nothing but does still parse
      _t.pop();
      return true;

    case Token::Type::Underscore: {
      // Unexpected `_`; report error
      auto tok = _t.pop();
      Log::get().error(tok->span,
        "expected an expression; found %s", arrow::to_string(tok->type).c_str());

      return false;
    }

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
    case Token::Type::Identifier:
      return parse_identifier();

    case Token::Type::None:
    case Token::Type::Integer:
    case Token::Type::Float:
    case Token::Type::String:
    case Token::Type::True:
    case Token::Type::False:
      return parse_literal();

    case Token::Type::LeftParenthesis:
      return parse_tuple();

    case Token::Type::LeftBracket:
      return parse_array();

    case Token::Type::If:
    case Token::Type::Unless:
      return parse_select();

    case Token::Type::LeftBrace:
      return parse_block(/*top_level=*/false);

    case Token::Type::Break:
      return parse_break();

    case Token::Type::Continue:
      return parse_continue();

    case Token::Type::Return:
      return parse_return();

    default:
      return false;
  }
}

// None
// -----------------------------------------------------------------------------
// none = "none" ;
// -----------------------------------------------------------------------------
bool Parser::parse_none() {
  auto tok = expect(Token::Type::None);
  if (!tok) return false;

  _stack.push_front(new ast::None(tok->span));

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

      case Token::Type::LeftBracket:
        if (!parse_index()) { return false; }
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

// Index
// -----------------------------------------------------------------------------
bool Parser::parse_index() {
  // Pull the awaiting operand expression
  auto expr = _stack.front();
  _stack.pop_front();

  // Expect `[`
  if (!expect(Token::Type::LeftBracket)) { return false; }

  // Parse the expression
  if (!parse_expression()) { return false; }
  auto index = _stack.front();
  _stack.pop_front();

  // Expect `]`
  if (!expect(Token::Type::RightBracket)) { return false; }

  // Declare and push the node
  _stack.push_front(new ast::Index(
    expr->span.extend(index->span), expr, index));

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
  _stack.push_front(new ast::Path(expr->span.extend(id->span), expr, id->text));

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
    expr = _stack.front();
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

  if (_t.peek(pindex)->type == Token::Type::Def) {
    return parse_extern_function();
  }

  return parse_extern_slot();
}

// Import
// -----------------------------------------------------------------------------
// import = "import" IDENTIFIER [ "from" STRING ] ";" ;
// -----------------------------------------------------------------------------
bool Parser::parse_import() {
  // Expect `import`
  auto initial_tok = expect(Token::Type::Import);
  if (!initial_tok) return false;

  // Parse the identifier for the imported module name
  auto id = expect<ast::Identifier>(&Parser::parse_identifier);
  if (!id) return false;
  auto name = id->text;

  // Check for `from` (which would contain an explicit source)
  auto source = name;
  if (_t.peek()->type == Token::Type::From) {
    _t.pop();

    auto str = expect<ast::String>(&Parser::parse_string);
    if (!str) return false;
    source = str->text;
  }

  // Expect `;`
  auto last_tok = expect(Token::Type::Semicolon);
  if (!last_tok) return false;

  // Declare and push the node
  _stack.push_front(new ast::Import(
    initial_tok->span.extend(last_tok->span),
    name,
    source
  ));

  return true;
}

// Alias
// -----------------------------------------------------------------------------
// alias = ["export"] "use" IDENTIFIER "=" type ;
// -----------------------------------------------------------------------------
bool Parser::parse_alias() {
  // Check for "export"
  Ref<Token> initial_tok = nullptr;
  bool exported = false;
  if (_t.peek()->type == Token::Type::Export) {
    exported = true;
    initial_tok = _t.pop();
  }

  // Expect `use`
  auto tok = expect(Token::Type::Use);
  if (!tok) return false;
  if (!initial_tok) initial_tok = tok;

  // Parse the identifier
  auto id = expect<ast::Identifier>(&Parser::parse_identifier);
  if (!id) return false;
  auto name = id->text;

  // Expect `=`
  if (!expect(Token::Type::Equals)) return false;

  // Expect type
  auto type = expect(&Parser::parse_type);
  if (!type) return false;

  // Expect `;`
  auto last_tok = expect(Token::Type::Semicolon);
  if (!last_tok) return false;

  // Declare and push the node
  _stack.push_front(new ast::Alias(
    initial_tok->span.extend(last_tok->span),
    exported,
    name,
    type
  ));

  return true;
}

}  // namespace arrow
