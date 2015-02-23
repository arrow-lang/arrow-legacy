#ifndef ARROW_PARSER_H
#define ARROW_PARSER_H 1

#include <cstdarg>
#include <deque>
#include "arrow/tokenizer.hpp"
#include "arrow/ast.hpp"

namespace arrow {

  class Parser {
  public:
    /// Constructs a `parser` bound to a tokenizer.
    Parser(Tokenizer& t);

    /// Parse an AST node from the bound tokenizer.
    std::shared_ptr<ast::Node> parse();

  private:
    template <typename T = arrow::Token>
    std::shared_ptr<T> expect(arrow::Type type) {
      return std::static_pointer_cast<T>(do_expect({type}));
    }

    template <typename T = arrow::Token>
    std::shared_ptr<T> expect(const std::vector<arrow::Type>& types) {
      return std::static_pointer_cast<T>(do_expect(types));
    }

    std::shared_ptr<arrow::Token> do_expect(
      const std::vector<arrow::Type>& types);

    bool parse_module_statement();
    bool parse_statement();
    bool parse_expression_statement();
    bool parse_expression();
    bool parse_postfix_expression();
    bool parse_unary_expression();
    bool parse_integer();
    bool parse_boolean();
    bool parse_break();

    Tokenizer& _t;

    /// Node stack; nodes get pushed as they are realized and popped as consumed.
    std::deque<std::shared_ptr<ast::Node>> _stack;

  };

}

#endif // ARROW_PARSER_H
