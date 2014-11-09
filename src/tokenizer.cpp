#include "arrow/tokenizer.hpp"

using arrow::Tokenizer;

Tokenizer::Tokenizer(const std::string& filename)
  : _filename(filename), _buffer(filename) {
}

auto Tokenizer::next() -> Token {
  auto byte = this->_buffer.next();

  // Check for an EOF
  // if (ch == 0) {
  //   return Token(Type::End);
  // }

  return Token(Type::End, Span("./test.as", Position(0, 0), Position(0, 1)));
}
