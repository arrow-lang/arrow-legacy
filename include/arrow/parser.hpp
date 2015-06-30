// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_PARSER_H
#define ARROW_PARSER_H 1

#include <cstdarg>
#include <deque>
#include <vector>

#include "arrow/ref.hpp"
#include "arrow/token.hpp"
#include "arrow/buffer.hpp"
#include "arrow/tokenizer.hpp"
#include "arrow/ast.hpp"

namespace arrow {

class Parser {
 public:
  Parser(std::shared_ptr<std::istream> is, const std::string& filename);

  Ref<ast::Node> parse();

 private:
  template <typename T>
  Ref<T> expect(Token::Type type) {
    return expect(type).as<T>();
  }

  template <typename T>
  Ref<T> expect(std::vector<Token::Type> types) {
    return expect(types).as<T>();
  }

  Ref<Token> expect(Token::Type type);
  Ref<Token> expect(std::vector<Token::Type> types);

  template <typename T>
  Ref<T> expect(bool (arrow::Parser::*cb)()) {
    return expect(cb).as<T>();
  }

  Ref<ast::Node> expect(bool (arrow::Parser::*cb)());

  bool do_sequence(Token::Type terminator, std::function<bool()> iter);
  // bool do_binary_expression(unsigned prec = 0, unsigned assoc = 0);

  bool parse_expression(unsigned power = 0);
  bool parse_unary_expression();
  int parse_binary_expression(unsigned power);

  bool parse_pattern(bool require_binding = false);

  bool parse_block(bool top_level = false);
  bool parse_module_statement();
  bool parse_statement();
  bool parse_expression_statement();
  bool parse_postfix_expression();
  bool parse_path();
  bool parse_call();
  bool parse_loop();
  bool parse_break();
  bool parse_continue();
  bool parse_return();
  bool parse_function();
  bool parse_extern();
  bool parse_extern_function();
  bool parse_extern_slot();
  bool parse_import();
  bool parse_struct();
  bool parse_slot();
  bool parse_select();
  // bool parse_expression();
  bool parse_primary_expression();
  bool parse_paren_expression();
  bool parse_integer();
  bool parse_boolean();
  bool parse_float();
  bool parse_string();
  bool parse_identifier();
  bool parse_type();
  bool parse_tuple();

  Tokenizer _t;

  /// Node stack; nodes get pushed as they are realized and popped as consumed.
  std::deque<Ref<ast::Node>> _stack;

};

}  // namespace arrow

#endif  // ARROW_PARSER_H
