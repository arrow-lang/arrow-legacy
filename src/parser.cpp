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
// statement = break;
// ----------------------------------------------------------------------------
bool Parser::parse_statement()
{
  return parse_break();
}

// Break
// ----------------------------------------------------------------------------
// break = "break" ";";
// ----------------------------------------------------------------------------
bool Parser::parse_break()
{
  // Expect `break`
  if (_t.peek(0)->type != Type::Break) {
    return false;
  }

  // Pop the token
  _t.pop();

  // Construct the node
  auto node = make_shared<ast::Break>();

  // Expect `;`
  auto tok = _t.peek(0);
  if (tok->type != Type::Semicolon) {
    Log::get().error(
      "unexpected %s (expected `;`)", to_string(tok->type).c_str());

    return false;
  }

  // Pop the token
  _t.pop();

  // Push the node
  _stack.push_back(node);

  return true;
}
