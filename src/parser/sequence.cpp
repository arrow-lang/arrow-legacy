// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/parser.hpp"

namespace arrow {

bool Parser::do_sequence(Token::Type terminator, std::function<bool()> iter) {
  bool ret = true;
  bool consume = false;
  while ((_t.peek()->type != Token::Type::End) &&
         (_t.peek()->type != terminator)) {
    if (!iter()) { consume = true; ret = false; break; }

    // Peek and consume the `,` token if present
    if (_t.peek()->type == Token::Type::Comma) {
      if (!expect(Token::Type::Comma)) { consume = true; ret = false; break; }
      continue;
    } else if ((_t.peek()->type != Token::Type::End) &&
               (_t.peek()->type != terminator)) {
      if (!expect(terminator)) { ret = false; break; }
    } else {
      break;
    }
  }

  if (!ret && consume) {
    // Attempt to consume all tokens until we find a
    // terminator token
    for (;;) {
      if (_t.peek()->type == terminator) {
        _t.pop();
        break;
      } else if (_t.peek()->type == Token::Type::End) {
        break;
      } else {
        _t.pop();
      }
    }
  }

  return ret;
}

}  // namespace arrow
