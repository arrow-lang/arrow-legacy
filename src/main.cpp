// #include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <memory>
#include <cstdint>
#include "arrow/buffer.hpp"
#include "arrow/span.hpp"
#include "arrow/token.hpp"

using arrow::Buffer;
using arrow::Position;
using arrow::Span;
using arrow::Token;
using arrow::Type;

class Tokenizer {
public:
  // Tokenizer(std::istream&& istream) noexcept;
  Tokenizer(const std::string& filename) noexcept
    : _buffer(filename) {
  }

  Token next() {
    auto ch = this->_buffer.next();

    // Check for an EOF
    // if (ch == 0) {
    //   return Token(Type::End);
    // }

    return Token(Type::End, Span("./test.as", Position(0, 0), Position(0, 1)));
  }

private:
  Buffer _buffer;

};

int main() {
  // arrow < {input} > {output}
  // tokenize!

  auto tokenizer = Tokenizer("./test.as");

  for (;;) {
    auto tok = tokenizer.next();

    std::printf("%s: ", tok.span.to_string().c_str());
    std::printf("note: ");
    std::printf("%s\n", arrow::to_string(tok.type).c_str());

    if (tok.type == Type::End) { break; }
  }
}
