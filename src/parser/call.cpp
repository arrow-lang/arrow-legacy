// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/parser.hpp"

namespace arrow {

// Call
// -----------------------------------------------------------------------------
// call = postfix-expression "(" [ call-arguments ] ")" ;
// call-arguments = (call-pos-arguments "," call-kw-arguments
//                   | call-kw-arguments
//                   | call-pos-arguments) [ "," ] ;
// call-pos-arguments = expression { "," expression } ;
// call-kw-arguments = call-kw-argument { "," call-kw-argument } ;
// call-kw-argument = IDENTIFIER "=" expression ;
// -----------------------------------------------------------------------------
bool Parser::parse_call() {
  // Pull the awaiting operand expression
  auto expr = _stack.front();
  _stack.pop_front();

  // Declare node
  Ref<ast::Call> call = new ast::Call(expr->span.extend(expr->span), expr);

  // Expect `(`
  auto tok = expect(Token::Type::LeftParenthesis);
  if (!tok) { return false; }

  // Iterate and parse each argument in the sequence
  bool in_kwargs = false;
  if (!do_sequence(Token::Type::RightParenthesis, [&, this]() {
    // Check for an identifier (to make a named/keyword argument)
    Ref<ast::Identifier> id = nullptr;
    Span span(_t.filename(), {0, 0}, {0, 0});
    bool kwargs_error = false;
    if (_t.peek(0)->type == Token::Type::Identifier &&
        _t.peek(1)->type == Token::Type::Equals) {
      id = expect<ast::Identifier>(&Parser::parse_identifier);
      if (!id) return false;

      // Expect an `=` to continue the named argument into the expression
      if (!expect(Token::Type::Equals)) return false;

      span = id->span;
      in_kwargs = true;
    } else if (in_kwargs) {
      kwargs_error = true;
    }

    // Parse the argument expression
    // TODO: Improve expect to handle this case
    if (!parse_expression()) return false;
    auto expr = _stack.front();
    _stack.pop_front();

    if (kwargs_error) {
      // If we didn't get a named argument but have had named
      // arguments; this is an error
      Log::get().error(
        expr->span, "non-keyword argument after keyword argument");

      return false;
    }

    // Push the argument
    std::string name = (id == nullptr) ? "" : id->text;
    span = in_kwargs ? span.extend(expr->span) : expr->span;
    call->arguments.push_back(new ast::Argument(span, expr, name));

    return true;
  })) return false;

  // Expect `)`
  if (!(tok = expect(Token::Type::RightParenthesis))) { return false; }

  // Extend the span of the call node
  call->span = call->span.extend(tok->span);

  // Declare (and push) the node
  _stack.push_front(call);

  return true;
}

}  // namespace arrow
