#ifndef ARROW_PARSER_H
#define ARROW_PARSER_H 1

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
    bool parse_module_statement();
    bool parse_statement();

    Tokenizer& _t;

    /// Node stack; nodes get pushed as they are realized and popped as consumed.
    std::deque<std::shared_ptr<ast::Node>> _stack;

  };

}

#endif // ARROW_PARSER_H
