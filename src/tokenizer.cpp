#include <cctype>
#include <sstream>
#include "arrow/tokenizer.hpp"

using arrow::Tokenizer;

Tokenizer::Tokenizer(const std::string& filename)
  : _filename(filename), _buffer(filename), _row(0), _column(0)
{
}

auto Tokenizer::_pos() const -> Position
{
  return Position(_row, _column);
}

auto Tokenizer::_make_token(Type type, Position begin, Position end) const
  -> std::shared_ptr<Token>
{
  return std::make_shared<Token>(
    type, Span(_filename, begin, end)
  );
}

bool Tokenizer::_is_eol(bool consume)
{
  auto eol = 0;
  if (_buffer.peek(0) == 0x0a) {  // ASCII LF (Linux)
    eol = 1;
  } else if (_buffer.peek(0) == 0x0d) {  // ASCII CR (Mac OS)
    eol = 1;
    if (_buffer.peek(1) == 0x0a) {  // ASCII CR + LF (Dos / Windows)
      eol += 1;
    }
  }

  auto test = !!eol;
  while (consume && eol > 0) {
    _buffer.pop();
    eol -= 1;
  }

  return test;
}

std::uint8_t Tokenizer::_buffer_next()
{
  // Get the next byte from the buffer ..
  auto byte = _buffer.next();

  // .. increment our column counter
  _column += 1;

  // .. and return the byte
  return byte;
}

auto Tokenizer::next() -> std::shared_ptr<Token>
{
  // Check for the end-of-stream condition ..
  if (_buffer.empty()) {
    // Reached end-of-stream, signal and get out
    return _make_token(Type::End, _pos(), _pos() + 1);
  }

  // Consume all whitespace
  while (std::isblank(_buffer.peek())) { _buffer_next(); }

  // Check if we are at a single-line comment indicator and
  // consume the comment.
  auto in_comment = false;
  if (_buffer.peek(0) == 0x23) {
    in_comment = true;
    _buffer_next();
  }

  if (in_comment) {
    for (;;) {
      // Check if we are at an end-of-line and stop consumption
      if (_is_eol()) { break; }

      // Consume this byte
      _buffer_next();
    }
  }

  // Check for an end-of-line condition ..
  if (_is_eol(/*consume=*/true)) {
    _column = 0;
    _row += 1;

    return next();
  }

  // Scan for a numeric (if we are on a digit) ..
  if (std::isdigit(_buffer.peek())) { return _scan_numeric(); }

  // Scan for a punctuator (and return the token if we match one) ..
  auto punc_tok = _scan_punctuator();
  if (punc_tok) { return punc_tok; }

  // Reached the end; report an unknown token (and consume it).
  auto cur = _pos();
  _buffer_next();
  return _make_token(Type::Unknown, cur, _pos());
}

/// Test `byte` and check if it is within the expected range
static bool in_range(std::uint8_t byte, std::uint8_t begin, std::uint8_t end)
{
  return (byte >= begin) and (byte <= end);
}

auto Tokenizer::_scan_punctuator() -> std::shared_ptr<Token>
{
  // Peek 3 bytes ahead
  auto p0 = _buffer.peek(0);
  auto p1 = _buffer.peek(1);
  auto p2 = _buffer.peek(2);

  // Check for defined punctuators
  // Check for the leading byte then narrow it down, etc.
  auto type = Type::Unknown;
  auto len = 1;
  if (p0 == 0x2b) {  // ASCII `+`
    type = Type::Plus;

    if (p1 == 0x3d) {  // ASCII `=`
      type = Type::Plus_Equals;
      len = 2;
    }
  } else if (p0 == 0x2d) {  // ASCII `-`
    type = Type::Minus;

    if (p1 == 0x3d) {  // ASCII `=`
      type = Type::Minus_Equals;
      len = 2;
    }
  } else if (p0 == 0x2f) {  // ASCII `/`
    type = Type::LSlash;

    if (p1 == 0x2f) {  // ASCII `/`
      type = Type::LSlash_LSlash;
      len = 2;

      if (p2 == 0x3d) {  // ASCII `=`
        type = Type::LSlash_LSlash_Equals;
        len = 3;
      }
    } else if (p1 == 0x3d) {  // ASCII `=`
      type = Type::LSlash_Equals;
      len = 2;
    }
  } else if (p0 == 0x25) {  // ASCII `%`
    type = Type::Percent;

    if (p1 == 0x3d) {  // ASCII `=`
      type = Type::Percent_Equals;
      len = 2;
    }
  } else if (p0 == 0x7c) {  // ASCII `|`
    type = Type::Pipe;

    if (p1 == 0x3d) {  // ASCII `=`
      type = Type::Pipe_Equals;
      len = 2;
    }
  } else if (p0 == 0x5e) {  // ASCII `^`
    type = Type::Caret;

    if (p1 == 0x3d) {  // ASCII `=`
      type = Type::Caret_Equals;
      len = 2;
    }
  } else if (p0 == 0x26) {  // ASCII `&`
    type = Type::Ampersand;

    if (p1 == 0x3d) {  // ASCII `=`
      type = Type::Ampersand_Equals;
      len = 2;
    }
  } else if (p0 == 0x3c) {  // ASCII `<`
    type = Type::LessThan;

    if (p1 == 0x3d) {  // ASCII `=`
      type = Type::LessThan_Equals;
      len = 2;
    }
  } else if (p0 == 0x3e) {  // ASCII `>`
    type = Type::GreaterThan;

    if (p1 == 0x3d) {  // ASCII `=`
      type = Type::GreaterThan_Equals;
      len = 2;
    }
  } else if (p0 == 0x3d) {  // ASCII `=`
    type = Type::Equals;

    if (p1 == 0x3d) {  // ASCII `=`
      type = Type::Equals_Equals;
      len = 2;
    }
  } else if (p0 == 0x21) {  // ASCII `!`
    type = Type::ExclamationMark;

    if (p1 == 0x3d) {  // ASCII `=`
      type = Type::ExclamationMark_Equals;
      len = 2;
    }
  }

  // Did we find one ..
  if (type == Type::Unknown) {
    // No punctuator detected; get out
    return nullptr;
  }

  // Build and return the punctuator token.
  auto begin = _pos();

  for (auto i = 0; i < len; ++i) {
    _buffer_next();
  }

  return _make_token(type, begin, _pos());
}

auto Tokenizer::_scan_numeric() -> std::shared_ptr<Token>
{
  // Initialize the text buffer.
  std::stringstream text;

  // Store the initial position.
  auto begin = _pos();

  // Declare a var to store the inferred type.
  auto type = Type::Integer;
  auto base = 10;

  // Check for a base-prefixed numeric ..
  if (_buffer.peek(0) == 0x30) {  // ASCII 0
    // Determine what our base is ..
    auto byte = _buffer.peek(1);
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
      _buffer_next();
      _buffer_next();
    }
  }

  // Build (and execute) the number consumption function to consume
  // the integral part of the complete numeric.
  auto consume_number = [&] {
    for (;;) {
      // Peek at the next digit
      auto byte = _buffer.peek();

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
      _buffer_next();
    }
  };

  consume_number();

  // We are no longer at a numeric (within range)
  if (base == 10) {
    if (_buffer.peek(0) == 0x2e  // ASCII .
          and std::isdigit(_buffer.peek(1))) {
      // We have at least `.#`, we will continue into
      // a decimal numeric.
      type = Type::Float;

      // Push the `.` into the buffer.
      text << (char)(_buffer_next());

      // Consume the expected number (again).
      consume_number();
    }

    // Now we /could/ continue into scientific notation
    // with at least `[eE][+-]?[0-9]` matching.
    auto p0 = _buffer.peek(0);
    auto p1 = _buffer.peek(1);
    auto p2 = _buffer.peek(2);
    if ((p0 == 0x45 or p0 == 0x65)
          and (std::isdigit(p1)
            or ((p1 == 0x2b or p1 == 0x2d) and std::isdigit(p2)))) {
      // We know we are a decimal numeric.
      type = Type::Float;

      // Push the first two characters.
      text << (char)(_buffer_next());
      text << (char)(_buffer_next());

      // Consume the expected number (again).
      consume_number();
    }
  }

  // Construct and return the token.
  auto span = Span(_filename, begin, _pos());
  if (type == Type::Float) {
    return std::make_shared<FloatToken>(text.str(), span);
  } else {
    return std::make_shared<IntegerToken>(base, text.str(), span);
  }
}
