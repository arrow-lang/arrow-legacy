// #include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <memory>
#include <cstdint>
#include "arrow/buffer.hpp"

using std::uint8_t;
using arrow::Buffer;

enum class Type {
  End,
};

const char* type_name(Type type) {
  switch (type) {
    case Type::End:
      return "end";

    default:
      return "unknown";
  }
}

struct Position {
  Position(uint row, uint column)
    : row(row), column(column) {
  }

  uint row;
  uint column;
};

/// [begin, end)
struct Span {
  Span(const std::string& filename, Position begin, Position end)
    : filename(filename), begin(begin), end(end) {
  }

  std::string to_string() const noexcept {
    std::stringstream fmt;

    fmt << (this->begin.row + 1);
    fmt << ',';
    fmt << (this->begin.column + 1);
    fmt << '-';

    if (this->begin.row == this->end.row) {
      // line,column-column
      fmt << (this->end.column + 1);
    } else {
      // line,column-line,column
      fmt << (this->end.row + 1);
      fmt << ',';
      fmt << (this->end.column + 1);
    }

    return fmt.str();
  }

  std::string filename;
  Position begin;
  Position end;
};

struct Token {
  Token(Type type)
    : type(type), span(nullptr) {
  }

  Token(Type type, Span span)
    : type(type), span(new Span(span)) {
  }

  Type type;
  std::unique_ptr<Span> span;
};

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

    std::printf("%s:", tok.span->filename.c_str());
    std::printf("%s: ", tok.span->to_string().c_str());
    std::printf("note: ");
    std::printf("%s\n", type_name(tok.type));

    if (tok.type == Type::End) { break; }
  }
}
