#include <map>
#include "arrow/parser.hpp"
#include "arrow/log.hpp"

using arrow::Parser;
using arrow::Type;
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
  while (_t.peek()->type != Type::End) {
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

std::shared_ptr<arrow::Token> Parser::do_expect(const std::vector<Type>& types)
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
    case Type::Break:
      return parse_break();

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
  if (!expect(Type::Semicolon)) {
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
    case Type::Integer:
      return parse_integer();

    case Type::Float:
      return parse_float();

    case Type::True:
    case Type::False:
      return parse_boolean();

    default:
      // Unknown expression
      // TODO: Should we really report failure like this here?
      expect({
        Type::Integer,
        Type::Float,
        Type::True,
        Type::False,
      });

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
  auto tok = expect<IntegerToken>(Type::Integer);
  if (!tok) { return false; }

  // Declare (and push) the node
  _stack.push_front(make_shared<ast::Integer>(tok->text, tok->base));

  return true;
}

// Float
// ----------------------------------------------------------------------------
// float = FLOAT ;
// ----------------------------------------------------------------------------
bool Parser::parse_float()
{
  // Expect FLOAT
  auto tok = expect<FloatToken>(Type::Float);
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
  auto tok = expect({Type::True, Type::False});
  if (!tok) { return false; }

  // Declare (and push) the node
  _stack.push_front(make_shared<ast::Boolean>(tok->type == Type::True));

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
  if ((tok->type != Type::Plus) &&
      (tok->type != Type::Minus) &&
      (tok->type != Type::ExclamationMark) &&
      (tok->type != Type::Not)) {
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
    case Type::Plus:
      node = make_shared<ast::Promote>(operand);
      break;

    case Type::Minus:
      node = make_shared<ast::NegateNumeric>(operand);
      break;

    case Type::Not:
      node = make_shared<ast::NegateLogical>(operand);
      break;

    case Type::ExclamationMark:
      node = make_shared<ast::NegateBitwise>(operand);
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
// postfix-expression = primary-expression ;
// ----------------------------------------------------------------------------
bool Parser::parse_postfix_expression()
{
  // NOTE: There are no postfix expressions defined at this time.
  return parse_primary_expression();
}

// Break
// ----------------------------------------------------------------------------
// break = "break" ";";
// ----------------------------------------------------------------------------
bool Parser::parse_break()
{
  // Expect `break` `;`
  if (!expect(Type::Break) || !expect(Type::Semicolon)) { return false; }

  // Declare (and push) the node
  _stack.push_front(make_shared<ast::Break>());

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

  static const std::map<arrow::Type, unsigned> PREC = {
    {Type::Equals,                   30},
    {Type::Plus_Equals,              30},
    {Type::Minus_Equals,             30},
    {Type::Asterisk_Equals,          30},
    {Type::Slash_Equals,             30},
    {Type::Percent_Equals,           30},
    {Type::Slash_Slash_Equals,       30},
    {Type::Ampersand_Equals,         30},
    {Type::Caret_Equals,             30},
    {Type::Pipe_Equals,              30},

    {Type::And,                      60},
    {Type::Or,                       60},

    {Type::Equals_Equals,            90},
    {Type::ExclamationMark_Equals,   90},
    {Type::LessThan,                 90},
    {Type::LessThan_Equals,          90},
    {Type::GreaterThan_Equals,       90},
    {Type::GreaterThan,              90},

    {Type::Ampersand,               110},
    {Type::Caret,                   110},
    {Type::Pipe,                    110},

    {Type::Plus,                    120},
    {Type::Minus,                   120},

    {Type::Asterisk,                150},
    {Type::Slash,                   150},
    {Type::Percent,                 150},
    {Type::Slash_Slash,             150},
  };

  // 0 => Left / 1 => Right
  static const std::map<arrow::Type, unsigned> ASSOC = {
    {Type::Equals,                  1},
    {Type::Plus_Equals,             1},
    {Type::Minus_Equals,            1},
    {Type::Asterisk_Equals,         1},
    {Type::Slash_Equals,            1},
    {Type::Percent_Equals,          1},
    {Type::Slash_Slash_Equals,      1},
    {Type::Ampersand_Equals,        1},
    {Type::Caret_Equals,            1},
    {Type::Pipe_Equals,             1},

    {Type::And,                     0},
    {Type::Or,                      0},

    {Type::Equals_Equals,           0},
    {Type::ExclamationMark_Equals,  0},
    {Type::LessThan,                0},
    {Type::LessThan_Equals,         0},
    {Type::GreaterThan_Equals,      0},
    {Type::GreaterThan,             0},

    {Type::Ampersand,               0},
    {Type::Caret,                   0},
    {Type::Pipe,                    0},

    {Type::Plus,                    0},
    {Type::Minus,                   0},

    {Type::Asterisk,                0},
    {Type::Slash,                   0},
    {Type::Percent,                 0},
    {Type::Slash_Slash,             0},
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
      case Type::Equals:
        node = make_shared<ast::Assign>(lhs, rhs);
        break;

      case Type::Plus_Equals:
        node = make_shared<ast::AssignAdd>(lhs, rhs);
        break;

      case Type::Minus_Equals:
        node = make_shared<ast::AssignSub>(lhs, rhs);
        break;

      case Type::Asterisk_Equals:
        node = make_shared<ast::AssignMul>(lhs, rhs);
        break;

      case Type::Slash_Equals:
        node = make_shared<ast::AssignDiv>(lhs, rhs);
        break;

      case Type::Percent_Equals:
        node = make_shared<ast::AssignMod>(lhs, rhs);
        break;

      case Type::Slash_Slash_Equals:
        node = make_shared<ast::AssignIntDiv>(lhs, rhs);
        break;

      case Type::Ampersand_Equals:
        node = make_shared<ast::AssignBitAnd>(lhs, rhs);
        break;

      case Type::Caret_Equals:
        node = make_shared<ast::AssignBitXor>(lhs, rhs);
        break;

      case Type::Pipe_Equals:
        node = make_shared<ast::AssignBitOr>(lhs, rhs);
        break;

      case Type::And:
        node = make_shared<ast::And>(lhs, rhs);
        break;

      case Type::Or:
        node = make_shared<ast::Or>(lhs, rhs);
        break;

      case Type::Equals_Equals:
        node = make_shared<ast::EqualTo>(lhs, rhs);
        break;

      case Type::ExclamationMark_Equals:
        node = make_shared<ast::NotEqualTo>(lhs, rhs);
        break;

      case Type::LessThan:
        node = make_shared<ast::LessThan>(lhs, rhs);
        break;

      case Type::LessThan_Equals:
        node = make_shared<ast::LessThanOrEqualTo>(lhs, rhs);
        break;

      case Type::GreaterThan_Equals:
        node = make_shared<ast::GreaterThanOrEqualTo>(lhs, rhs);
        break;

      case Type::GreaterThan:
        node = make_shared<ast::GreaterThan>(lhs, rhs);
        break;

      case Type::Ampersand:
        node = make_shared<ast::BitAnd>(lhs, rhs);
        break;

      case Type::Caret:
        node = make_shared<ast::BitXor>(lhs, rhs);
        break;

      case Type::Pipe:
        node = make_shared<ast::BitOr>(lhs, rhs);
        break;

      case Type::Plus:
        node = make_shared<ast::Add>(lhs, rhs);
        break;

      case Type::Minus:
        node = make_shared<ast::Sub>(lhs, rhs);
        break;

      case Type::Asterisk:
        node = make_shared<ast::Mul>(lhs, rhs);
        break;

      case Type::Slash:
        node = make_shared<ast::Div>(lhs, rhs);
        break;

      case Type::Percent:
        node = make_shared<ast::Mod>(lhs, rhs);
        break;

      case Type::Slash_Slash:
        node = make_shared<ast::IntDiv>(lhs, rhs);
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
