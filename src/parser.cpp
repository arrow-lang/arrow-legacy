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

    Log::get().error(stream.str());

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
  if (!parse_unary_expression()) { return false; }

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
// expression = integer ;
// ----------------------------------------------------------------------------
bool Parser::parse_expression()
{
  switch (_t.peek()->type) {
    case Type::Integer:
      return parse_integer();

    case Type::True:
    case Type::False:
      return parse_boolean();

    default:
      // Unknown expression
      // TODO: Report unexpected ?
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

  // Declare the node
  auto node = make_shared<ast::Integer>(tok->text, tok->base);

  // Push the node
  _stack.push_front(node);

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

  // Declare the node
  auto node = make_shared<ast::Boolean>(tok->type == Type::True);

  // Push the node
  _stack.push_front(node);

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
  if (!parse_expression()) { return false; }
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
// postfix-expression = expression ;
// ----------------------------------------------------------------------------
bool Parser::parse_postfix_expression()
{
  // NOTE: There are no postfix expressions defined at this time.
  return parse_expression();
}

// Break
// ----------------------------------------------------------------------------
// break = "break" ";";
// ----------------------------------------------------------------------------
bool Parser::parse_break()
{
  // Expect `break`
  if (!expect(Type::Break)) { return false; }

  // Declare node
  auto node = make_shared<ast::Break>();

  // Expect `;`
  if (!expect(Type::Semicolon)) { return false; }

  // Push node
  _stack.push_front(node);

  return true;
}
