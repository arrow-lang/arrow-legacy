#include <cctype>
#include <sstream>
#include "arrow/tokenizer.hpp"

using arrow::Tokenizer;

Tokenizer::Tokenizer(const std::string& filename)
  : _filename(filename), _buffer(filename), _row(0), _column(0) {
}

auto Tokenizer::_position() const -> Position {
  return Position(this->_row, this->_column);
}

auto Tokenizer::_make(Type type, Position begin) const -> Token {
  return Token(type, Span(this->_filename, begin, this->_position()));
}

std::uint8_t Tokenizer::_buffer_next() {
  auto byte = this->_buffer.next();
  this->_column += 1;
  return byte;
}

auto Tokenizer::next() -> Token {
  auto cur = this->_position();

  // Request the next byte.
  auto byte = this->_buffer_next();

  // Check for the end-of-stream condition ..
  if (byte == 0) {
    // Reached end-of-stream, signal and get out
    return this->_make(Type::End, cur);
  }

  // Consume all whitespace
  while (std::isblank(byte)) {
    byte = this->_buffer_next();
  }

  // Check for an end-of-line condition ..
  auto eol = false;
  if (byte == 0x0a) {  // ASCII LF (Linux)
    eol = true;
  } else if (byte == 0x0d) {  // ASCII CR (Mac OS)
    eol = true;
    if (this->_buffer.peek() == 0x0a) {  // ASCII CR + LF (Dos / Windows)
      this->_buffer.pop();
    }
  }

  if (eol) {
    this->_column = 0;
    this->_row += 1;

    return this->next();
  }

  // Scan for a numeric ..
  if (std::isdigit(byte)) {

    // Iterate and consume the complete numeric.
    std::stringstream number;
    number << (char)byte;
    while (std::isdigit(this->_buffer.peek())) {
      number << (char)(this->_buffer_next());
    }

    return this->_make(Type::Integer, cur);

    // if (byte == 0x30 and
    //     ((this->_buffer.peek(0) == 0x78) or
    //      (this->_buffer.peek(1) == 0x58))) {  // ASCII 0 + (x or X)
    //   //
    // }


  }

  return this->_make(Type::Unknown, cur);
}
