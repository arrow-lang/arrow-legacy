// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <algorithm>

#include "arrow/parser.hpp"

namespace arrow {

Ref<Token> Parser::expect(Token::Type type) {
  return expect(std::vector<Token::Type>({type}));
}

Ref<Token> Parser::expect(std::vector<Token::Type> types) {
  auto tok = _t.pop();
  if (std::find(types.begin(), types.end(), tok->type) == types.end()) {
    // Failed to meet our condition; build error message
    std::stringstream st;

    st << "expected ";

    if (types.size() > 1) {
      st << "one of ";
    }

    for (unsigned i = 0; i < types.size(); ++i) {
      auto type = types.at(i);

      if (i > 0) {
        st << ", ";
      }

      if (i > 0 && i == (types.size() - 1)) {
        st << "or ";
      }

      st << arrow::to_string(type);
    }

    st << ", found " << arrow::to_string(tok->type);

    Log::get().error(tok->span, st.str().c_str());
    return nullptr;
  }

  return tok;
}

Ref<ast::Node> Parser::expect(bool (arrow::Parser::*cb)()) {
  if (!(this->*cb)()) return nullptr;
  auto res = _stack.front();
  _stack.pop_front();
  return res;
}

}  // namespace arrow
