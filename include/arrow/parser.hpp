// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_PARSER_H
#define ARROW_PARSER_H 1

#include <cstdarg>
#include <deque>
#include <vector>

#include "arrow/tokenizer.hpp"
#include "arrow/ast.hpp"

namespace arrow {

class Parser {
 public:
  /// Constructs a `parser` bound to a tokenizer.
  explicit Parser(Tokenizer& t);

  /// Parse an AST node from the bound tokenizer.
  std::shared_ptr<ast::Node> parse();

 private:
  template <typename T = arrow::Token>
  std::shared_ptr<T> expect(arrow::Token::Type type) {
    return std::static_pointer_cast<T>(do_expect({type}));
  }

  template <typename T = arrow::Token>
  std::shared_ptr<T> expect(const std::vector<arrow::Token::Type>& types) {
    return std::static_pointer_cast<T>(do_expect(types));
  }

  std::shared_ptr<arrow::Token> do_expect(
    const std::vector<arrow::Token::Type>& types);

  bool parse_module_statement();
  bool parse_statement();
  bool parse_expression_statement();
  bool parse_expression();
  bool parse_primary_expression();
  bool parse_postfix_expression();
  bool parse_unary_expression();
  bool parse_paren_expression();
  bool parse_integer();
  bool parse_float();
  bool parse_function();
  bool parse_extern_function();
  bool parse_identifier();
  bool parse_boolean();
  bool parse_string();
  bool parse_break();
  bool parse_return();
  bool parse_call_expression();
  bool parse_slot();
  bool parse_type();
  bool parse_pointer_type();
  bool parse_select();
  bool parse_loop();
  bool parse_import();

  bool parse_block(ast::Block& node);

  bool parse_function_parameter();
  bool parse_function_signature(ast::AbstractFunction& fn);
  bool parse_function_parameters(ast::AbstractFunction& fn);

  bool parse_binary_expression(unsigned prec = 0, unsigned assoc = 0);

  int parse_select_branch(ast::Select& x, unsigned index);

  Tokenizer& _t;

  /// Node stack; nodes get pushed as they are realized and popped as consumed.
  std::deque<std::shared_ptr<ast::Node>> _stack;
};

}  // namespace arrow

#endif  // ARROW_PARSER_H
