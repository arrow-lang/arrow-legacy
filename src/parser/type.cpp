// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/parser.hpp"

namespace arrow {

// Type
// -----------------------------------------------------------------------------
// type = identifier
//      | "None"
//      | "*" type
//      | "(" ")"
//      | "(" type "," ")"
//      | "(" type "," type { "," type } [ "," ] ")"
//      | function-type
//      ;
// -----------------------------------------------------------------------------
bool Parser::parse_type() {
  auto tok = _t.peek(0);

  if (!([&, this]() -> bool {
    // Check for `None` to indicate the unit type
    if (tok->type == Token::Type::None) {
      auto tok = _t.pop();

      _stack.push_front(new ast::TypeNone(tok->span));
      return true;
    }

    // Check for an identifier (for a simple type)
    if (tok->type == Token::Type::Identifier) {
      std::vector<std::string> segments;
      auto span = tok->span;

      for (unsigned i = 0; ; i++) {
        // Expect a `.` (unless this is the first identifier)
        if (i > 0 && !expect(Token::Type::Period)) return false;

        // Parse the identifier
        auto id_node = expect<ast::Identifier>(&Parser::parse_identifier);
        if (!id_node) return false;

        segments.push_back(id_node->text);

        // Extend the span
        span = span.extend(id_node->span);

        // Check for a `.` that could continue this into a path
        if (_t.peek(0)->type != Token::Type::Period) break;
      }

      _stack.push_front(new ast::TypePath(
        span,
        segments
      ));

      return true;
    }

    // Check for a `*` to indicate a pointer type
    if (tok->type == Token::Type::Asterisk) {
      _t.pop();

      // Check for `mutable` to indicate a mutable pointee
      bool is_mutable = false;
      if (_t.peek(0)->type == Token::Type::Mutable) {
        _t.pop();
        is_mutable = true;
      }

      // Parse the pointee type
      auto pointee = expect(&Parser::parse_type);
      if (!pointee) return false;

      _stack.push_front(new ast::TypePointer(
        tok->span.extend(pointee->span),
        pointee,
        is_mutable
      ));

      return true;
    }

    // Check for a `(` which would be the start of a tuple type OR a
    // function type
    if (tok->type == Token::Type::LeftParenthesis) {
      _t.pop();

      // Declare the tuple-type node
      Ref<ast::TypeTuple> node = new ast::TypeTuple(tok->span);

      // Check for an immediate `)` which makes this an empty tuple
      if (_t.peek(0)->type == Token::Type::RightParenthesis) {
        tok = _t.pop();
        node->span = node->span.extend(tok->span);

        // Check for a "->" (which would make this definitely be
        // a function type)
        if (_t.peek(0)->type == Token::Type::Arrow) {
          return parse_type_function(node, false);
        }

        _stack.push_front(node);
        return true;
      }

      // Check for a named parameter (which would make this definitely be
      // a function type)
      if (_t.peek(0)->type == Token::Type::Identifier &&
          _t.peek(1)->type == Token::Type::Colon) {
        return parse_type_function(node);
      }

      // Parse the first element (which is a special case for tuples)
      auto elem0 = expect(&Parser::parse_type);
      if (!elem0) return false;
      node->elements.push_back(elem0);

      // Check for an arrow (which would make this definitely be
      // a function type)
      if (_t.peek(0)->type == Token::Type::RightParenthesis &&
          _t.peek(1)->type == Token::Type::Arrow) {
        _t.pop();
        return parse_type_function(node, false);
      }

      // Check for an immediate `)` which makes this just a grouping
      if (_t.peek()->type == Token::Type::RightParenthesis) {
        _t.pop();
        _stack.push_front(elem0);

        return true;
      }

      // Expect a comma to follow the first element (to prove we're a tuple)
      if (!expect(Token::Type::Comma)) return false;

      // Iterate and parse each remaining element in the sequence
      if (!do_sequence(Token::Type::RightParenthesis, [&, this]() {
        // Check for a named parameter (which would make this definitely be
        // a function type)
        if (_t.peek(0)->type == Token::Type::Identifier &&
            _t.peek(1)->type == Token::Type::Colon) {
          return parse_type_function(node);
        }

        // Parse the element
        auto elem = expect(&Parser::parse_type);
        if (!elem) return false;

        // Push the element
        node->elements.push_back(elem);

        return true;
      })) return false;

      // Expect `)`
      if (!(tok = expect(Token::Type::RightParenthesis))) { return false; }
      node->span = node->span.extend(tok->span);

      // Check for a "->" (which would make this definitely be
      // a function type)
      if (_t.peek(0)->type == Token::Type::Arrow) {
        return parse_type_function(node, false);
      }

      // Push the node
      _stack.push_front(node);

      return true;
    }

    // TODO: type(X)

    // Fail; could not match a pattern
    // TODO: Should probably have shorter error message
    expect({
      Token::Type::Identifier,
      Token::Type::LeftParenthesis,
      Token::Type::Asterisk,
      Token::Type::None
    });

    return false;
  })()) { return false; }

  // Attempt to match successive `[..]` to make this an array type
  while (_t.peek()->type == Token::Type::LeftBracket) {
    _t.pop();

    // Pull the element type expression
    auto element = _stack.front();
    _stack.pop_front();

    // Check for an immediate `]` to indicate an unsized or dynamic array
    if (_t.peek()->type == Token::Type::RightBracket) {
      auto last_tok = _t.pop();

      _stack.push_front(new ast::TypeArray(
        element->span.extend(last_tok->span),
        element
      ));
    }

    // Parse an expression for the size
    if (!parse_expression()) return false;
    auto size = _stack.front();
    _stack.pop_front();

    // Expect an `]` to close the array type
    auto last_tok = expect(Token::Type::RightBracket);

    _stack.push_front(new ast::TypeArray(
      element->span.extend(last_tok->span),
      element,
      size
    ));
  }

  return true;
}

// Type Function
// -----------------------------------------------------------------------------
// function-type-parameter = [ "identifier" ":" ] type ;
// function-type = "(" ")" "->" type
//               | "(" function-type-parameter { "," function-type-parameter } [ "," ] ")" "->" type
//               ;
// -----------------------------------------------------------------------------
bool Parser::parse_type_function(Ref<ast::TypeTuple> initial, bool in_params) {
  // Declare the tuple-type node
  Ref<ast::TypeFunction> node = new ast::TypeFunction(initial->span);

  // Consume parsed type parameters (so far)
  for (auto& elem : initial->elements) {
    node->parameters.push_back(new ast::TypeParameter(
      elem->span,
      "",
      elem
    ));
  }

  if (in_params) {
    // Iterate and parse each parameter in the sequence
    if (!do_sequence(Token::Type::RightParenthesis, [&, this]() {
      // Check for `IDENTIFIER ":"` to allow for the keyword
      auto begin = _t.peek(0)->span;
      std::string keyword = "";
      if (_t.peek(0)->type == Token::Type::Identifier &&
          _t.peek(1)->type == Token::Type::Colon) {
        auto id = expect<ast::Identifier>(&Parser::parse_identifier);
        keyword = id->text;
        _t.pop(); // ":"
      }

      // Parse the element
      auto type = expect(&Parser::parse_type);
      if (!type) return false;

      // Push the element
      node->parameters.push_back(new ast::TypeParameter(
        begin.extend(type->span),
        keyword,
        type
      ));

      return true;
    })) return false;

    // Expect a `)`
    auto tok = expect(Token::Type::RightParenthesis);
    if (!tok) return false;
    node->span = node->span.extend(tok->span);
  }

  // Expect a `->`
  auto tok = expect(Token::Type::Arrow);
  if (!tok) return false;
  node->span = node->span.extend(tok->span);

  node->result = expect<ast::Type>(&Parser::parse_type);
  if (!node->result) return false;

  // Push the node
  _stack.push_front(node);

  return true;
}

}  // namespace arrow
