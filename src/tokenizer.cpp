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

std::uint8_t Tokenizer::_buffer_next() {
  auto byte = this->_buffer.next();
  this->_column += 1;
  return byte;
}

auto Tokenizer::next() -> std::shared_ptr<Token> {
  // Check for the end-of-stream condition ..
  if (this->_buffer.empty()) {
    // Reached end-of-stream, signal and get out
    return std::make_shared<Token>(
      Type::End, Span(
        this->_filename, this->_position(), this->_position() + 1));
  }

  // Consume all whitespace
  while (std::isblank(this->_buffer.peek())) { this->_buffer_next(); }

  // Check if we are at a single-line comment indicator and
  // consume the comment.
  auto in_comment = false;
  if (this->_buffer.peek(0) == 0x23) {
    in_comment = true;
    this->_buffer_next();
  } else if (this->_buffer.peek(0) == 0x2f and this->_buffer.peek(1) == 0x2f) {
    in_comment = true;
    this->_buffer_next();
    this->_buffer_next();
  }

  if (in_comment) {
    for (;;) {
      // Check if we are at an end-of-line and stop consumption
      auto byte = this->_buffer.peek();
      if (byte == 0x0a or byte == 0x0d) { break; }

      // Consume this byte
      this->_buffer_next();
    }
  }

  // Check for an end-of-line condition ..
  auto eol = false;
  if (this->_buffer.peek() == 0x0a) {  // ASCII LF (Linux)
    this->_buffer.pop();
    eol = true;
  } else if (this->_buffer.peek() == 0x0d) {  // ASCII CR (Mac OS)
    eol = true;
    this->_buffer.pop();
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
  if (std::isdigit(this->_buffer.peek())) {
    return this->_scan_numeric();
  }

  // Reached the end; report an unknown token.
  auto pos = this->_position();
  this->_buffer_next();
  return std::make_shared<Token>(
    Type::Unknown, Span(this->_filename, pos, this->_position()));
}

/// Test `byte` and check if it is within the expected range
static bool in_range(std::uint8_t byte, std::uint8_t begin, std::uint8_t end) {
  return (byte >= begin) and (byte <= end);
}

auto Tokenizer::_scan_numeric() -> std::shared_ptr<Token> {
  // Initialize the text buffer.
  std::stringstream text;

  // Store the initial position.
  auto pos = this->_position();

  // Declare a var to store the inferred type.
  auto type = Type::Integer;
  auto base = 10;

  // Check for a base-prefixed numeric ..
  if (this->_buffer.peek(0) == 0x30) {  // ASCII 0
    // Determine what our base is ..
    auto byte = this->_buffer.peek(1);
    auto prefix = true;
    if (byte == 0x58 or byte == 0x78) {  // ASCII x OR X
      // Hexadecimal
      base = 16;
    } else if (byte == 0x42 or byte == 0x62) {  // ASCII b OR B
      // Binary
      base = 2;
    } else if (byte == 0x4F or byte == 0x6F) {  // ASCII o OR O
      // Octal
      base = 8;
    } else {
      // Not base-prefixed
      prefix = false;
    }

    // Pop the two prefix characters (if prefixed)
    if (prefix) {
      this->_buffer_next();
      this->_buffer_next();
    }
  }

  // Build (and execute) the number consumption function to consume
  // the integral part of the complete numeric.
  auto consume_number = [&] {
    for (;;) {
      // Peek at the next digit
      auto byte = this->_buffer.peek();

      // Check if this is a valid digit (for our base)
      if (base == 16) {
        if (!std::isxdigit(byte)) {
          break;
        }
      } else if (!in_range(byte, '0', '0' + (base - 1))) {
        break;
      }

      // Push it into the buffer
      text << (char)byte;

      // Advance the input buffer
      this->_buffer_next();
    }
  };

  consume_number();

  // We are no longer at a numeric (within range)
  if (base == 10) {
    if (this->_buffer.peek(0) == 0x2e  // ASCII .
          and std::isdigit(this->_buffer.peek(1))) {
      // We have at least `.#`, we will continue into
      // a decimal numeric.
      type = Type::Float;

      // Push the `.` into the buffer.
      text << (char)(this->_buffer_next());

      // Consume the expected number (again).
      consume_number();
    }

    // Now we /could/ continue into scientific notation
    // with at least `[eE][+-]?[0-9]` matching.
    auto p0 = this->_buffer.peek(0);
    auto p1 = this->_buffer.peek(1);
    auto p2 = this->_buffer.peek(2);
    if ((p0 == 0x45 or p0 == 0x65)
          and (std::isdigit(p1)
            or ((p1 == 0x2b or p1 == 0x2d) and std::isdigit(p2)))) {
      // We know we are a decimal numeric.
      type = Type::Float;

      // Push the first two characters.
      text << (char)(this->_buffer_next());
      text << (char)(this->_buffer_next());

      // Consume the expected number (again).
      consume_number();
    }
  }

  // Construct and return the token.
  auto span = Span(this->_filename, pos, this->_position());
  if (type == Type::Float) {
    return std::make_shared<FloatToken>(text.str(), span);
  } else {
    return std::make_shared<IntegerToken>(base, text.str(), span);
  }
}
