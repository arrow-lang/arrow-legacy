#include <vector>
#include <iterator>
#include <cctype>
#include <sstream>
#include "arrow/tokenizer.hpp"
#include "utf8.h"

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

  // Scan for an identifier (or a token) ..
  auto ident_tok = _scan_identifier();
  if (ident_tok) { return ident_tok; }

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
  switch (p0) {
    case 0x3a:  // ASCII `:`
      type = Type::Colon;
      break;

    case 0x3b:  // ASCII `;`
      type = Type::Semicolon;
      break;

    case 0x28:  // ASCII `(`
      type = Type::LeftParenthesis;
      break;

    case 0x29:  // ASCII `)`
      type = Type::RightParenthesis;
      break;

    case 0x5b:  // ASCII `[`
      type = Type::LeftBracket;
      break;

    case 0x5d:  // ASCII `]`
      type = Type::RightBracket;
      break;

    case 0x7b:  // ASCII `{`
      type = Type::LeftBrace;
      break;

    case 0x7d:  // ASCII `}`
      type = Type::RightBrace;
      break;

    case 0x2b:  // ASCII `+`
      type = Type::Plus;

      if (p1 == 0x3d) {  // ASCII `=`
        type = Type::Plus_Equals;
        len = 2;
      }

      break;

    case 0x2d:  // ASCII `-`
      type = Type::Minus;

      if (p1 == 0x3d) {  // ASCII `=`
        type = Type::Minus_Equals;
        len = 2;
      }

      break;

    case 0x2a:  // ASCII `*`
      type = Type::Asterisk;

      if (p1 == 0x3d) {  // ASCII `=`
        type = Type::Asterisk_Equals;
        len = 2;
      }

      break;

    case 0x25:  // ASCII `%`
      type = Type::Percent;

      if (p1 == 0x3d) {  // ASCII `=`
        type = Type::Percent_Equals;
        len = 2;
      }

      break;

    case 0x2f:  // ASCII `/`
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

      break;

    case 0x7c:  // ASCII `|`
      type = Type::Pipe;

      if (p1 == 0x3d) {  // ASCII `=`
        type = Type::Pipe_Equals;
        len = 2;
      }

      break;

    case 0x5e:  // ASCII `^`
      type = Type::Caret;

      if (p1 == 0x3d) {  // ASCII `=`
        type = Type::Caret_Equals;
        len = 2;
      }

      break;

    case 0x26:  // ASCII `&`
      type = Type::Ampersand;

      if (p1 == 0x3d) {  // ASCII `=`
        type = Type::Ampersand_Equals;
        len = 2;
      }

      break;

    case 0x3c:  // ASCII `<`
      type = Type::LessThan;

      if (p1 == 0x3d) {  // ASCII `=`
        type = Type::LessThan_Equals;
        len = 2;
      }

      break;

    case 0x3e:  // ASCII `>`
      type = Type::GreaterThan;

      if (p1 == 0x3d) {  // ASCII `=`
        type = Type::GreaterThan_Equals;
        len = 2;
      }

      break;

    case 0x3d:  // ASCII `=`
      type = Type::Equals;

      if (p1 == 0x3d) {  // ASCII `=`
        type = Type::Equals_Equals;
        len = 2;
      }

      break;

    case 0x21:  // ASCII `!`
      type = Type::ExclamationMark;

      if (p1 == 0x3d) {  // ASCII `=`
        type = Type::ExclamationMark_Equals;
        len = 2;
      }

      break;

    default:
      // Nothing found; get out
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

std::uint32_t Tokenizer::_buffer_peek_utf32(unsigned offset, unsigned* count) {
  // The basic idea here is to peek bytes until we have an entire
  // valid UTF-32
  auto cnt = 0;
  auto valid = false;
  std::vector<std::uint8_t> bytes;

  while (!valid && cnt < 4) {
    bytes.push_back(_buffer.peek(offset + cnt));
    cnt += 1;
    valid = utf8::is_valid(bytes.begin(), bytes.end());
  }

  if (valid) {
    std::vector<std::uint32_t> res;
    utf8::utf8to32(bytes.begin(), bytes.end(), std::back_inserter(res));

    if (count) {
      *count = cnt;
    }

    return res[0];

  } else {
    return 0;
  }
}

static bool in_ranges(
  std::uint32_t value, std::vector<std::vector<std::uint32_t>>& ranges)
{
  for (auto& range : ranges) {
    auto min = range[0];
    auto max = range[1];

    if ((value >= min) && (value <= max)) {
      return true;
    }
  }

  return false;
}

auto Tokenizer::_scan_identifier() -> std::shared_ptr<Token>
{
  // REF: http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1518.htm

  // An identifier can contain ..
  static std::vector<std::vector<std::uint32_t>> can_contain = {
    {0x30, 0x39},
    {0x41, 0x5A},
    {0x5F, 0x5F},
    {0x61, 0x7A},

    {0x00A8, 0x00A8},
    {0x00AA, 0x00AA},
    {0x00AD, 0x00AD},
    {0x00AF, 0x00AF},
    {0x00B2, 0x00B5},
    {0x00B7, 0x00BA},
    {0x00BC, 0x00BE},
    {0x00C0, 0x00D6},
    {0x00D8, 0x00F6},
    {0x00F8, 0x00FF},

    {0x0100, 0x167F},
    {0x1681, 0x180D},
    {0x180F, 0x1FFF},
    {0x200B, 0x200D},
    {0x202A, 0x202E},
    {0x203F, 0x2040},
    {0x2054, 0x2054},
    {0x2060, 0x206F},

    {0x2070, 0x218F},
    {0x2460, 0x24FF},
    {0x2776, 0x2793},
    {0x2C00, 0x2DFF},
    {0x2E80, 0x2FFF},

    {0x3004, 0x3007},
    {0x3021, 0x302F},
    {0x3031, 0x303F},

    {0x3040, 0xD7FF},

    {0xF900, 0xFD3D},
    {0xFD40, 0xFDCF},
    {0xFDF0, 0xFE44},
    {0xFE47, 0xFFFD},

    {0x10000, 0x1FFFD},
    {0x20000, 0x2FFFD},
    {0x30000, 0x3FFFD},
    {0x40000, 0x4FFFD},
    {0x50000, 0x5FFFD},
    {0x60000, 0x6FFFD},
    {0x70000, 0x7FFFD},
    {0x80000, 0x8FFFD},
    {0x90000, 0x9FFFD},
    {0xA0000, 0xAFFFD},
    {0xB0000, 0xBFFFD},
    {0xC0000, 0xCFFFD},
    {0xD0000, 0xDFFFD},
    {0xE0000, 0xEFFFD},
  };

  // An identifier must not start with (of those sets) ..
  static std::vector<std::vector<std::uint32_t>> not_start_with = {
    {0x30, 0x39},
    {0x0300, 0x036F},
    {0x1DC0, 0x1DFF},
    {0x20D0, 0x20FF},
    {0xFE20, 0xFE2F},
  };

  // Mark our current position
  auto begin = _pos();

  // Build our UTF8 identifier
  std::vector<std::uint8_t> bytes;
  unsigned total_count = 0;
  unsigned count = 0;

  for (;;) {
    // Peek the next UTF-32 character
    auto ch = _buffer_peek_utf32(total_count, &count);
    if (ch == 0) { break; }

    // Is this one of our "can-contain" characters
    if (!in_ranges(ch, can_contain)) {
      // No.. tough luck
      break;
    }

    // Are we at the beginning and is this one of our "must-not-start-with"
    // characters
    if (total_count == 0 && in_ranges(ch, not_start_with)) {
      // Yep.. tough luck
      break;
    }

    // Increment total counter and append our character
    total_count += count;
    utf8::append(ch, std::back_inserter(bytes));
  }

  if (total_count == 0) {
    // Got nothing
    return nullptr;
  }

  // We found something.. pop the consumed bytes
  for (unsigned i = 0; i < total_count; ++i) { _buffer_next(); }

  // Create our token
  auto span = Span(_filename, begin, _pos());
  return std::make_shared<IdentifierToken>(
    std::string(reinterpret_cast<const char*>(bytes.data()), bytes.size()),
    span);
}
