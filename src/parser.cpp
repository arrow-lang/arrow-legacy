#include <map>
#include "arrow/parser.hpp"
#include "arrow/log.hpp"

using arrow::Parser;
using arrow::Token;
namespace ast = arrow::ast;

using std::make_shared;

Parser::Parser(Tokenizer& t)
  : _t(t), _stack()
{
}

std::shared_ptr<ast::Node> Parser::parse()
{
  // Declare the top-level (root) module
  auto module = make_shared<ast::Module>();

  // Enumerate and attempt to match rules until the token stream
  // is empty
  while (_t.peek()->type != Token::Type::End) {
    // Try and parse a module statement ..
    if (parse_module_statement()) {
      // Consume the parsed stack
      module->sequence.insert(
        module->sequence.end(), _stack.begin(), _stack.end());
    }

    // Clear the (parsed) stack
    _stack.clear();
  }

  return module;
}

std::shared_ptr<arrow::Token> Parser::do_expect(
  const std::vector<Token::Type>& types)
{
  auto tok = _t.peek(0);
  bool found = false;
  for (auto& type : types) {
    if (tok->type == type) {
      found = true;
      break;
    }
  }

  if (found) {
    _t.pop();

    return tok;
  } else {
    std::stringstream stream;
    stream
      << "unexpected "
      << arrow::to_string(tok->type)
      << " (expected ";

    for (unsigned i = 0; i < types.size(); ++i) {
      auto type = types[i];

      if (i > 0) {
        stream << ", ";
      }

      if ((types.size() > 1) && (i == (types.size() - 1))) {
        stream << "or ";
      }

      stream << arrow::to_string(type);
    }

    stream << ")";

    Log::get().error(tok->span, stream.str());

    return nullptr;
  }
}

// Module Statement
// A `statement` that can only be found in the top-level of a module.
// ----------------------------------------------------------------------------
// module-statement = statement ;
// ----------------------------------------------------------------------------
bool Parser::parse_module_statement()
{
  return parse_statement();
}

// Statement
// ----------------------------------------------------------------------------
// statement = break
//           | expression-statement
//           ;
// ----------------------------------------------------------------------------
bool Parser::parse_statement()
{
  switch (_t.peek()->type) {
    case Token::Type::Break:
      return parse_break();

    case Token::Type::Return:
      return parse_return();

    case Token::Type::Def:
      return parse_function();

    default:
      // We must be an `expression statement`
      return parse_expression_statement();
  }
}

// Expression Statement
// ----------------------------------------------------------------------------
// expression-statement = unary-expression ";" ;
// ----------------------------------------------------------------------------
bool Parser::parse_expression_statement()
{
  if (!parse_expression()) { return false; }

  // Expect `;`
  if (!expect(Token::Type::Semicolon)) {
    // Clear the (parsed) stack
    _stack.clear();

    return false;
  }

  return true;
}

// Expression
// ----------------------------------------------------------------------------
// expression = unary-expression | binary-expression ;
// ----------------------------------------------------------------------------
bool Parser::parse_expression()
{
  // Attempt to parse the [..] expression
  if (!parse_unary_expression()) { return false; }

  // Attempt to continue the parsed [..] expression into a binary expression
  if (!parse_binary_expression()) { return false; }

  return true;
}

// Primary Expression
// ----------------------------------------------------------------------------
// primary-expression = integer | float | boolean ;
// ----------------------------------------------------------------------------
bool Parser::parse_primary_expression()
{
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

    default:
      // Unknown expression
      return false;
  }
}

// Integer
// ----------------------------------------------------------------------------
// integer = INTEGER ;
// ----------------------------------------------------------------------------
bool Parser::parse_integer()
{
  // Expect INTEGER
  auto tok = expect<IntegerToken>(Token::Type::Integer);
  if (!tok) { return false; }

  // Declare (and push) the node
  _stack.push_front(make_shared<ast::Integer>(tok->text, tok->base));

  return true;
}

// String
// ----------------------------------------------------------------------------
// string = STRING ;
// ----------------------------------------------------------------------------
bool Parser::parse_string()
{
  // Expect STRING
  auto tok = expect<StringToken>(Token::Type::String);
  if (!tok) { return false; }

  // Declare (and push) the node
  _stack.push_front(make_shared<ast::String>(tok->bytes));

  return true;
}

// Float
// ----------------------------------------------------------------------------
// float = FLOAT ;
// ----------------------------------------------------------------------------
bool Parser::parse_float()
{
  // Expect FLOAT
  auto tok = expect<FloatToken>(Token::Type::Float);
  if (!tok) { return false; }

  // Declare (and push) the node
  _stack.push_front(make_shared<ast::Float>(tok->text));

  return true;
}

// Boolean
// ----------------------------------------------------------------------------
// boolean = BOOLEAN ;
// ----------------------------------------------------------------------------
bool Parser::parse_boolean()
{
  // Expect INTEGER
  auto tok = expect({Token::Type::True, Token::Type::False});
  if (!tok) { return false; }

  // Declare (and push) the node
  _stack.push_front(make_shared<ast::Boolean>(tok->type == Token::Type::True));

  return true;
}

// Unary Expression
// ----------------------------------------------------------------------------
// unary-expression = unary-operator postfix-expression ;
// unary-operator = "+" | "-" | "not" | "!" ;
// ----------------------------------------------------------------------------
bool Parser::parse_unary_expression()
{
  // If this is NOT a unary expression ..
  auto tok = _t.peek(0);
  if ((tok->type != Token::Type::Plus) &&
      (tok->type != Token::Type::Minus) &&
      (tok->type != Token::Type::ExclamationMark) &&
      (tok->type != Token::Type::Not)) {
    // .. we MAY be a postfix expression
    return parse_postfix_expression();
  }

  // Pop the unary operator token
  _t.pop();

  // Parse the operand
  if (!parse_unary_expression()) { return false; }
  auto operand = _stack.front();
  _stack.pop_front();

  // Declare the node
  std::shared_ptr<ast::Node> node;
  switch (tok->type) {
    case Token::Type::Plus:
      node = make_shared<ast::Promote>(operand);
      break;

    case Token::Type::Minus:
      node = make_shared<ast::NegateNumeric>(operand);
      break;

    case Token::Type::Not:
      node = make_shared<ast::NegateLogical>(operand);
      break;

    case Token::Type::ExclamationMark:
      node = make_shared<ast::NegateBit>(operand);
      break;

    default:
      // Unreachable
      break;
  }

  // Push the node
  _stack.push_front(node);

  return true;
}

// Postfix Expression
// ----------------------------------------------------------------------------
// postfix-expression = primary-expression
//                    | call-expression
//                    ;
// ----------------------------------------------------------------------------
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
        if (!parse_call_expression()) { return false; }
        break;

      default:
        continue_ = false;
        break;
    }
  }

  return true;
}

// Call Expression
// ----------------------------------------------------------------------------
// call-expression = postfix-expression "(" ")" ;
// ----------------------------------------------------------------------------
bool Parser::parse_call_expression() {
  // Pull the awaiting operand expression
  auto expr = _stack.front();
  _stack.pop_front();

  // Expect `(`
  if (!expect(Token::Type::LeftParenthesis)) { return false; }

  // TODO: Iterate and parse parameters

  // Expect `)`
  if (!expect(Token::Type::RightParenthesis)) { return false; }

  // Declare (and push) the node
  _stack.push_front(make_shared<ast::Call>(expr));

  return true;
}

// Break
// ----------------------------------------------------------------------------
// break = "break" ";";
// ----------------------------------------------------------------------------
bool Parser::parse_break() {
  // Expect `break` `;`
  if (!expect(Token::Type::Break) || !expect(Token::Type::Semicolon)) {
    return false;
  }

  // Declare (and push) the node
  _stack.push_front(make_shared<ast::Break>());

  return true;
}

// Identifier
// ----------------------------------------------------------------------------
// identifier = IDENTIFIER ;
// ----------------------------------------------------------------------------
bool Parser::parse_identifier()
{
  // Expect IDENTIFIER
  auto tok = expect<IdentifierToken>(Token::Type::Identifier);
  if (!tok) { return false; }

  // Declare (and push) the node
  _stack.push_front(make_shared<ast::Identifier>(tok->text));

  return true;
}

// Return
// ----------------------------------------------------------------------------
// return = "return" expression ";" ;
// ----------------------------------------------------------------------------
bool Parser::parse_return()
{
  // Expect `return`
  if (!expect(Token::Type::Return)) { return false; }

  // Attempt to parse the returned expression
  if (!parse_expression()) { return false; }
  auto expr = _stack.front();
  _stack.pop_front();

  // Expect `;`
  if (!expect(Token::Type::Semicolon)) { return false; }

  // Declare (and push) the node
  _stack.push_front(make_shared<ast::Return>(expr));

  return true;
}

// Binary Expression
// ----------------------------------------------------------------------------
// binary-expression = expression binary-operator expression ;
// binary-operator = "+" | "-" | "*" | "/" | "%" | "//" | "^" | "&" | "|"
//                 | "==" | "!=" | "<" | "<=" | ">" | ">="
//                 | "and" | "or"
//                 | "=" | "+=" | "-=" | "*=" | "/=" | "%=" | "//="
//                 | "^=" | "&=" | "|="
//                 ;
// ----------------------------------------------------------------------------
bool Parser::parse_binary_expression(unsigned prec, unsigned assoc)
{
  // NOTE: This method is not normal. This is intended to be called
  //  directly after parsing a `unary-expression`. This is not LR parsing
  //  but instead a precedence parser.

  static const std::map<Token::Type, unsigned> PREC = {
    {Token::Type::Equals,                   30},
    {Token::Type::Plus_Equals,              30},
    {Token::Type::Minus_Equals,             30},
    {Token::Type::Asterisk_Equals,          30},
    {Token::Type::Slash_Equals,             30},
    {Token::Type::Percent_Equals,           30},
    {Token::Type::Ampersand_Equals,         30},
    {Token::Type::Caret_Equals,             30},
    {Token::Type::Pipe_Equals,              30},

    {Token::Type::And,                      60},
    {Token::Type::Or,                       60},

    {Token::Type::Equals_Equals,            90},
    {Token::Type::ExclamationMark_Equals,   90},
    {Token::Type::LessThan,                 90},
    {Token::Type::LessThan_Equals,          90},
    {Token::Type::GreaterThan_Equals,       90},
    {Token::Type::GreaterThan,              90},

    {Token::Type::Ampersand,               110},
    {Token::Type::Caret,                   110},
    {Token::Type::Pipe,                    110},

    {Token::Type::Plus,                    120},
    {Token::Type::Minus,                   120},

    {Token::Type::Asterisk,                150},
    {Token::Type::Slash,                   150},
    {Token::Type::Percent,                 150},
  };

  // 0 => Left / 1 => Right
  static const std::map<Token::Type, unsigned> ASSOC = {
    {Token::Type::Equals,                  1},
    {Token::Type::Plus_Equals,             1},
    {Token::Type::Minus_Equals,            1},
    {Token::Type::Asterisk_Equals,         1},
    {Token::Type::Slash_Equals,            1},
    {Token::Type::Percent_Equals,          1},
    {Token::Type::Ampersand_Equals,        1},
    {Token::Type::Caret_Equals,            1},
    {Token::Type::Pipe_Equals,             1},

    {Token::Type::And,                     0},
    {Token::Type::Or,                      0},

    {Token::Type::Equals_Equals,           0},
    {Token::Type::ExclamationMark_Equals,  0},
    {Token::Type::LessThan,                0},
    {Token::Type::LessThan_Equals,         0},
    {Token::Type::GreaterThan_Equals,      0},
    {Token::Type::GreaterThan,             0},

    {Token::Type::Ampersand,               0},
    {Token::Type::Caret,                   0},
    {Token::Type::Pipe,                    0},

    {Token::Type::Plus,                    0},
    {Token::Type::Minus,                   0},

    {Token::Type::Asterisk,                0},
    {Token::Type::Slash,                   0},
    {Token::Type::Percent,                 0},
  };

  for (;;) {

    // Get the token and its precedence and associativity
    auto tok = _t.peek(0);
    auto tok_prec = PREC.find(tok->type);
    auto tok_assoc = ASSOC.find(tok->type);

    // If the next token is not a binary operator token
    // or the token binds less tightly and is left-associative,
    // get out of the precedence parser.
    if (tok_prec == PREC.end()) { return true; }
    if ((tok_prec->second < prec) && assoc == 0) { return true; }

    // Get the LHS
    auto lhs = _stack.front();
    _stack.pop_front();

    // Pop the operand token
    _t.pop();

    // Parse the RHS
    if (!parse_unary_expression()) { return false; }

    // If the binary operator binds less tightly with RHS than the
    // operator after RHS, let the pending operator take RHS as its LHS.
    auto next_prec = PREC.find(_t.peek(0)->type);
    if (next_prec != PREC.end() && (
        (tok_prec->second < next_prec->second) ||
        (tok_assoc->second == 1 && (tok_prec->second == next_prec->second)))) {
      if (!parse_binary_expression((tok_prec->second) + 1, tok_assoc->second)) {
        return false;
      }
    }

    // Get the RHS
    auto rhs = _stack.front();
    _stack.pop_front();

    // Declare the node
    std::shared_ptr<ast::Node> node;
    switch (tok->type) {
      case Token::Type::Equals:
        node = make_shared<ast::Assign>(lhs, rhs);
        break;

      case Token::Type::Plus_Equals:
        node = make_shared<ast::AssignAdd>(lhs, rhs);
        break;

      case Token::Type::Minus_Equals:
        node = make_shared<ast::AssignSub>(lhs, rhs);
        break;

      case Token::Type::Asterisk_Equals:
        node = make_shared<ast::AssignMul>(lhs, rhs);
        break;

      case Token::Type::Slash_Equals:
        node = make_shared<ast::AssignDiv>(lhs, rhs);
        break;

      case Token::Type::Percent_Equals:
        node = make_shared<ast::AssignMod>(lhs, rhs);
        break;

      case Token::Type::Ampersand_Equals:
        node = make_shared<ast::AssignBitAnd>(lhs, rhs);
        break;

      case Token::Type::Caret_Equals:
        node = make_shared<ast::AssignBitXor>(lhs, rhs);
        break;

      case Token::Type::Pipe_Equals:
        node = make_shared<ast::AssignBitOr>(lhs, rhs);
        break;

      case Token::Type::And:
        node = make_shared<ast::And>(lhs, rhs);
        break;

      case Token::Type::Or:
        node = make_shared<ast::Or>(lhs, rhs);
        break;

      case Token::Type::Equals_Equals:
        node = make_shared<ast::EqualTo>(lhs, rhs);
        break;

      case Token::Type::ExclamationMark_Equals:
        node = make_shared<ast::NotEqualTo>(lhs, rhs);
        break;

      case Token::Type::LessThan:
        node = make_shared<ast::LessThan>(lhs, rhs);
        break;

      case Token::Type::LessThan_Equals:
        node = make_shared<ast::LessThanOrEqualTo>(lhs, rhs);
        break;

      case Token::Type::GreaterThan_Equals:
        node = make_shared<ast::GreaterThanOrEqualTo>(lhs, rhs);
        break;

      case Token::Type::GreaterThan:
        node = make_shared<ast::GreaterThan>(lhs, rhs);
        break;

      case Token::Type::Ampersand:
        node = make_shared<ast::BitAnd>(lhs, rhs);
        break;

      case Token::Type::Caret:
        node = make_shared<ast::BitXor>(lhs, rhs);
        break;

      case Token::Type::Pipe:
        node = make_shared<ast::BitOr>(lhs, rhs);
        break;

      case Token::Type::Plus:
        node = make_shared<ast::Add>(lhs, rhs);
        break;

      case Token::Type::Minus:
        node = make_shared<ast::Sub>(lhs, rhs);
        break;

      case Token::Type::Asterisk:
        node = make_shared<ast::Mul>(lhs, rhs);
        break;

      case Token::Type::Slash:
        node = make_shared<ast::Div>(lhs, rhs);
        break;

      case Token::Type::Percent:
        node = make_shared<ast::Mod>(lhs, rhs);
        break;

      default:
        // Unreachable
        return false;
    }

    // Push the node
    _stack.push_front(node);
  }

  // Unreachable
  return false;
}

// Function (Declaration)
// ----------------------------------------------------------------------------
// function = "def" identifier "(" ")" block ;
// ----------------------------------------------------------------------------
bool Parser::parse_function()
{
  // Expect `def`
  if (!expect(Token::Type::Def)) { return false; }

  // Parse identifier
  if (!parse_identifier()) { return false; }
  auto name = std::static_pointer_cast<ast::Identifier>(_stack.front());
  _stack.pop_front();

  // Expect `(`
  if (!expect(Token::Type::LeftParenthesis)) { return false; }

  // Expect `)`
  if (!expect(Token::Type::RightParenthesis)) { return false; }

  // Expect `{`
  if (!expect(Token::Type::LeftBrace)) { return false; }

  // Declare the node
  auto fn = make_shared<ast::Function>(name);

  // Enumerate and attempt to match rules until we reach
  // `}` or the end of stream (which would be an error)
  while ((_t.peek()->type != Token::Type::End) &&
         (_t.peek()->type != Token::Type::RightBrace)) {
    // Try and parse a statement ..
    if (parse_statement()) {
      // Consume the parsed stack
      fn->sequence.insert(fn->sequence.end(), _stack.begin(), _stack.end());
    }

    // Clear the (parsed) stack
    _stack.clear();
  }

  // Expect `}`
  if (!expect(Token::Type::RightBrace)) { return false; }

  // Push the node
  _stack.push_front(fn);

  return true;
}
