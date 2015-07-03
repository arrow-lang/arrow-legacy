// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/parser.hpp"

// [ ] Cover all operators
// [ ] Postfix expressions
// [ ] Remaining primary expressions

namespace arrow {

static std::map<Token::Type, unsigned> UNARY = {
  // Identitiy, negation, bitwise not, dereference, and address-of [2]
  {Token::Type::Plus,            2000},
  {Token::Type::Minus,           2000},
  {Token::Type::ExclamationMark, 2000},
  {Token::Type::Asterisk,        2000},
  {Token::Type::Ampersand,       2000},

  // Logical NOT [11]
  {Token::Type::Not,             1100},
};

// First part of the pair is the token power; second is the
// associativity (1 for left-to-right and -1 for right-to-left).
static std::map<Token::Type, std::pair<unsigned, unsigned>> BINARY = {
  // Multiplication, division, and remainder [3]
  {Token::Type::Asterisk,                {1900,  1}},
  {Token::Type::Percent,                 {1900,  1}},
  {Token::Type::Slash,                   {1900,  1}},

  // Addition and subtraction [4]
  {Token::Type::Plus,                    {1800,  1}},
  {Token::Type::Minus,                   {1800,  1}},

  // Bitwise AND [5]
  {Token::Type::Ampersand,               {1700,  1}},

  // Bitwise XOR [6]
  {Token::Type::Caret,                   {1600,  1}},

  // Bitwise OR [7]
  {Token::Type::Pipe,                    {1500,  1}},

  // Greater than; and, greater than or equal to [8]
  {Token::Type::GreaterThan_Equals,      {1400,  1}},
  {Token::Type::GreaterThan,             {1400,  1}},

  // Less than; and, less than or equal to [8]
  {Token::Type::LessThan,                {1400,  1}},
  {Token::Type::LessThan_Equals,         {1400,  1}},

  // Cast [9]
  {Token::Type::As,                      {1300,  1}},

  // Equal to; and, not equal to [10]
  {Token::Type::Equals_Equals,           {1200,  1}},
  {Token::Type::ExclamationMark_Equals,  {1200,  1}},

  // Logical AND [12]
  {Token::Type::And,                     {1000,  1}},

  // Logical OR [13]
  {Token::Type::Or,                      { 900,  1}},

  // Conditional Expression [14]
  {Token::Type::If,                      { 800,  1}},

  // Assignment [15]
  {Token::Type::Equals,                  { 700, -1}},
  {Token::Type::Plus_Equals,             { 700, -1}},
  {Token::Type::Minus_Equals,            { 700, -1}},
  {Token::Type::Asterisk_Equals,         { 700, -1}},
  {Token::Type::Slash_Equals,            { 700, -1}},
  {Token::Type::Percent_Equals,          { 700, -1}},
  {Token::Type::Ampersand_Equals,        { 700, -1}},
  {Token::Type::Caret_Equals,            { 700, -1}},
  {Token::Type::Pipe_Equals,             { 700, -1}},
};

bool Parser::parse_expression(unsigned power /* = 0 */) {
  // Size of the stack when we started the power iteration
  auto initial_size = _stack.size();

  for (;;) {
    // Look ahead at the next token
    auto tok = _t.peek(0);
    if (tok->type == Token::Type::End) { break; }

    // Attempt to match a unary token (but only if we don't have
    // something waiting to be matched in the stack)
    auto uref = UNARY.find(tok->type);
    if ((initial_size == _stack.size()) && (uref != UNARY.end())) {
      if (!parse_unary_expression()) return false;
      continue;
    }

    // Attempt to match a binary token
    if (_stack.size() > 0 && BINARY.find(tok->type) != BINARY.end()) {
      auto rv = parse_binary_expression(power);
      if      (rv == -1) return false;
      else if (rv ==  0) continue;
      else break;
    }

    // Attempt to match a postfix expression (which could be
    // a primary expression).
    if (_stack.size() >= 1) break;
    if (!parse_postfix_expression()) {
      // Fail only if we haven't parsed anything
      if (initial_size != _stack.size()) {
        return true;
      } else {
        break;
      }
    }
  }

  // Check if we managed to parse anything
  if (initial_size == _stack.size()) {
    // No; report an error
    auto tok = _t.pop();
    Log::get().error(tok->span,
      "expected an expression; found %s", arrow::to_string(tok->type).c_str());

    return false;
  }

  return true;
}

int Parser::parse_binary_expression(unsigned power) {
  // Check binary
  auto tok = _t.peek(0);
  auto tok_power = BINARY[tok->type].first;
  auto tok_assoc = BINARY[tok->type].second;

  // Check if we should consume this token
  if (tok_power < power) return 1;

  // Handle binary
  _t.pop();

  // Check if we have a previous expression (lhs)
  if (_stack.size() == 0) {
    // Report an error (that we missed an expression)
    Log::get().error(tok->span,
      "expected an expression; found %s", arrow::to_string(tok->type).c_str());

    return false;
  }

  // Get the previous expression (lhs)
  auto lhs = _stack.front();
  _stack.pop_front();

  // Determine what the RHS should be and parse
  if (tok->type == Token::Type::As) {
    // Parse the RHS (which must be a type)
    if (!parse_type()) return -1;
  } else {
    // Request an expression (rhs)
    // TODO: Improve `expect` to handle this case
    if (!parse_expression(tok_power + tok_assoc)) return -1;
  }

  auto rhs = _stack.front();
  _stack.pop_front();

  // Declare and push the node
  auto sp = lhs->span.extend(rhs->span);
  Ref<ast::Node> node = nullptr;
  switch (tok->type) {
    case Token::Type::As:
      node = new ast::Cast(sp, lhs, rhs);
      break;

    case Token::Type::Plus:
      node = new ast::Add(sp, lhs, rhs);
      break;

    case Token::Type::Minus:
      node = new ast::Sub(sp, lhs, rhs);
      break;

    case Token::Type::Asterisk:
      node = new ast::Mul(sp, lhs, rhs);
      break;

    case Token::Type::Slash:
      node = new ast::Div(sp, lhs, rhs);
      break;

    case Token::Type::Percent:
      node = new ast::Mod(sp, lhs, rhs);
      break;

    case Token::Type::Equals:
      node = new ast::Assign(sp, lhs, rhs);
      break;

    case Token::Type::Ampersand:
      node = new ast::BitAnd(sp, lhs, rhs);
      break;

    case Token::Type::Caret:
      node = new ast::BitXor(sp, lhs, rhs);
      break;

    case Token::Type::Pipe:
      node = new ast::BitOr(sp, lhs, rhs);
      break;

    case Token::Type::Equals_Equals:
      node = new ast::EqualTo(sp, lhs, rhs);
      break;

    case Token::Type::ExclamationMark_Equals:
      node = new ast::NotEqualTo(sp, lhs, rhs);
      break;

    case Token::Type::GreaterThan_Equals:
      node = new ast::GreaterThanOrEqualTo(sp, lhs, rhs);
      break;

    case Token::Type::GreaterThan:
      node = new ast::GreaterThan(sp, lhs, rhs);
      break;

    case Token::Type::LessThan_Equals:
      node = new ast::LessThanOrEqualTo(sp, lhs, rhs);
      break;

    case Token::Type::LessThan:
      node = new ast::LessThan(sp, lhs, rhs);
      break;

    case Token::Type::And:
      node = new ast::And(sp, lhs, rhs);
      break;

    case Token::Type::Or:
      node = new ast::Or(sp, lhs, rhs);
      break;

    case Token::Type::Plus_Equals:
      node = new ast::Assign(sp, lhs, new ast::Add(sp, lhs, rhs));
      break;

    case Token::Type::Minus_Equals:
      node = new ast::Assign(sp, lhs, new ast::Sub(sp, lhs, rhs));
      break;

    case Token::Type::Asterisk_Equals:
      node = new ast::Assign(sp, lhs, new ast::Mul(sp, lhs, rhs));
      break;

    case Token::Type::Slash_Equals:
      node = new ast::Assign(sp, lhs, new ast::Div(sp, lhs, rhs));
      break;

    case Token::Type::Percent_Equals:
      node = new ast::Assign(sp, lhs, new ast::Mod(sp, lhs, rhs));
      break;

    case Token::Type::Ampersand_Equals:
      node = new ast::Assign(sp, lhs, new ast::BitAnd(sp, lhs, rhs));
      break;

    case Token::Type::Caret_Equals:
      node = new ast::Assign(sp, lhs, new ast::BitXor(sp, lhs, rhs));
      break;

    case Token::Type::Pipe_Equals:
      node = new ast::Assign(sp, lhs, new ast::BitOr(sp, lhs, rhs));
      break;

    default:
      // Unreachable
      break;
  }

  _stack.push_front(node);

  return 0;
}

bool Parser::parse_unary_expression() {
  // Handle unary
  auto tok = _t.pop();
  auto tok_power = UNARY[tok->type];

  // Check for `mutable` (to indicate a mutable adress of)
  bool mut = false;
  if (tok->type == Token::Type::Ampersand) {
    if (_t.peek(0)->type == Token::Type::Mutable) {
      mut = true;
      _t.pop();
    }
  }

  // Request an expression (operand)
  // TODO: Improve `expect` to handle this case
  if (!parse_expression(tok_power + 1)) return false;
  auto operand = _stack.front();
  _stack.pop_front();

  // Declare and push the node
  auto sp = tok->span.extend(operand->span);
  Ref<ast::Node> node = nullptr;
  switch (tok->type) {
    case Token::Type::Plus:
      node = new ast::Identity(sp, operand);
      break;

    case Token::Type::Minus:
      node = new ast::Negate(sp, operand);
      break;

    case Token::Type::ExclamationMark:
      node = new ast::BitNot(sp, operand);
      break;

    case Token::Type::Ampersand:
      node = new ast::AddressOf(sp, operand, mut);
      break;

    case Token::Type::Asterisk:
      node = new ast::Dereference(sp, operand);
      break;

    case Token::Type::Not:
      node = new ast::Not(sp, operand);
      break;

    default:
      // Unreachable
      break;
  }

  _stack.push_front(node);

  return true;
}

} // namespace arrow {
