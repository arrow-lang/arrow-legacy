// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <stddef.h>
#include <stdlib.h>

#include <gmp.h>

#include <functional>
#include <map>
#include <unordered_map>

#include "utfcpp/utf8.h"

#include "arrow/tokenizer.hpp"
#include "arrow/log.hpp"

using std::vector;
using std::uint32_t;

static bool is_whitespace(uint32_t ch) {
  switch (ch) {
    case 0x20:  // Space
    case 0x09:  // Tab
    case 0x0b:  // Vertical Tab
    case 0x0c:  // Form Feed
      // Considered a whitespace character (that is not a new-line)
      return true;

    default:
      // Not a whitespace character
      return false;
  }
}

/// Test and check if it is within the expected range
static bool in_range(uint32_t ch, uint32_t begin, uint32_t end) {
  return (ch >= begin) && (ch <= end);
}

static bool in_ranges(uint32_t value, const vector<vector<uint32_t>>& ranges) {
  for (auto& range : ranges) {
    auto min = range[0];
    auto max = range[1];

    if ((value >= min) && (value <= max)) {
      return true;
    }
  }

  return false;
}

// Keywords
static std::unordered_map<std::string, arrow::Token::Type> keywords = {
  {"None",      arrow::Token::Type::None        },
  {"type",      arrow::Token::Type::TypeOf      },
  {"and",       arrow::Token::Type::And         },
  {"or",        arrow::Token::Type::Or          },
  {"not",       arrow::Token::Type::Not         },
  {"let",       arrow::Token::Type::Let         },
  {"mutable",   arrow::Token::Type::Mutable     },
  {"def",       arrow::Token::Type::Def         },
  {"extern",    arrow::Token::Type::Extern      },
  {"export",    arrow::Token::Type::Export      },
  {"import",    arrow::Token::Type::Import      },
  {"from",      arrow::Token::Type::From        },
  {"true",      arrow::Token::Type::True        },
  {"false",     arrow::Token::Type::False       },
  {"if",        arrow::Token::Type::If          },
  {"unless",    arrow::Token::Type::Unless      },
  {"else",      arrow::Token::Type::Else        },
  {"loop",      arrow::Token::Type::Loop        },
  {"while",     arrow::Token::Type::While       },
  {"until",     arrow::Token::Type::Until       },
  {"break",     arrow::Token::Type::Break       },
  {"continue",  arrow::Token::Type::Continue    },
  {"return",    arrow::Token::Type::Return      },
  {"global",    arrow::Token::Type::Global      },
  {"as",        arrow::Token::Type::As          },
  {"struct",    arrow::Token::Type::Struct      },
  {"_",         arrow::Token::Type::Underscore  },
};

// Punctuators
static std::map<vector<uint32_t>, arrow::Token::Type> punctuators = {
  // 1-character
  {{'+'}, arrow::Token::Type::Plus             },
  {{'-'}, arrow::Token::Type::Minus            },
  {{'/'}, arrow::Token::Type::Slash            },
  {{'*'}, arrow::Token::Type::Asterisk         },
  {{'%'}, arrow::Token::Type::Percent          },
  {{'&'}, arrow::Token::Type::Ampersand        },
  {{'|'}, arrow::Token::Type::Pipe             },
  {{'^'}, arrow::Token::Type::Caret            },
  {{'!'}, arrow::Token::Type::ExclamationMark  },
  {{'='}, arrow::Token::Type::Equals           },
  {{'<'}, arrow::Token::Type::LessThan         },
  {{'>'}, arrow::Token::Type::GreaterThan      },
  {{'.'}, arrow::Token::Type::Period           },
  {{':'}, arrow::Token::Type::Colon            },
  {{';'}, arrow::Token::Type::Semicolon        },
  {{'{'}, arrow::Token::Type::LeftBrace        },
  {{'}'}, arrow::Token::Type::RightBrace       },
  {{'('}, arrow::Token::Type::LeftParenthesis  },
  {{')'}, arrow::Token::Type::RightParenthesis },
  {{'['}, arrow::Token::Type::LeftBracket      },
  {{']'}, arrow::Token::Type::RightBracket     },
  {{','}, arrow::Token::Type::Comma            },

  // 2-character
  {{'+', '='}, arrow::Token::Type::Plus_Equals             },
  {{'-', '='}, arrow::Token::Type::Minus_Equals            },
  {{'*', '='}, arrow::Token::Type::Asterisk_Equals         },
  {{'/', '='}, arrow::Token::Type::Slash_Equals            },
  {{'%', '='}, arrow::Token::Type::Percent_Equals          },
  {{'&', '='}, arrow::Token::Type::Ampersand_Equals        },
  {{'|', '='}, arrow::Token::Type::Pipe_Equals             },
  {{'^', '='}, arrow::Token::Type::Caret_Equals            },
  {{'=', '='}, arrow::Token::Type::Equals_Equals           },
  {{'!', '='}, arrow::Token::Type::ExclamationMark_Equals  },
  {{'>', '='}, arrow::Token::Type::GreaterThan_Equals      },
  {{'<', '='}, arrow::Token::Type::LessThan_Equals         },
  {{'-', '>'}, arrow::Token::Type::Arrow                   },
  {{'=', '>'}, arrow::Token::Type::FatArrow                },

  // 3-character
  {{'.', '.', '.'}, arrow::Token::Type::Ellipsis           },
};

namespace arrow {

Tokenizer::Tokenizer(
  std::shared_ptr<std::istream> is, const std::string& filename)
    : _filename(filename),
      _queue(),
      _b(is),
      _scanners() {
  // Initialize list of scanners
  _scanners.push_back(std::bind(&Tokenizer::_scan_numeric, this));
  _scanners.push_back(std::bind(&Tokenizer::_scan_string, this));
  _scanners.push_back(std::bind(&Tokenizer::_scan_punctuator, this));
  _scanners.push_back(std::bind(&Tokenizer::_scan_identifier, this));
}

auto Tokenizer::pop() -> Ref<Token> {
  if (!_read(1)) return nullptr;

  // Get (and consume) the next byte.
  // NOTE: Why can't C++ be sane and have `.pop` return a value
  auto tok = _queue.front();
  _queue.pop_front();
  return tok;
}

auto Tokenizer::peek(unsigned offset) -> Ref<Token> {
  if (!_read(offset + 1)) return nullptr;

  // Peek (and perserve) the `offset` token.
  return _queue.at(offset);
}

bool Tokenizer::_read(std::size_t count) {
  // If we've recursed enough; return
  // If we've enough tokens in the queue; return
  if (count == 0 || _queue.size() > count) return true;

  // Check for the end-of-stream condition ..
  if (_b.empty()) {
    // Reached end-of-stream, signal and get out
    _queue.push_back(new Token(
      Token::Type::End, Span(_filename, _b.pos(), 1)));

    return _read(count - 1);
  }

  // Consume all whitespace characters (excluding new lines).
  while (is_whitespace(_b.peek())) { _b.pop(); }

  // Check if we are at a single-line comment and consume it.
  if (_consume_line_comment()) { return _read(count); }

  // Check for and consume the end-of-line character.
  // TODO: Insert a semicolon token into the queue if the situation demands it
  if (_b.peek() == 0x0a) { _b.pop(); return _read(count); }

  // Iterate through our scanning algorithms and
  // continue until someone finds something
  for (auto& scan : _scanners) {
    auto tok = scan();
    if (tok) {
      // Found something; push it back
      _queue.push_back(tok);

      return _read(count - 1);
    }
  }

  // Reached the end; report an error for finding an
  // unknown token.
  auto ch = _b.pop();
  vector<char> bytes;
  utf8::append(ch, std::back_inserter(bytes));
  std::string text(bytes.data(), bytes.size());

  Log::get().error(Span(_filename, _b.pos() - 1, _b.pos()), "unexpected token: `%s`", text.c_str());

  // Keep going .. (until we get EOF)
  return _read(count);
}

bool Tokenizer::_consume_line_comment() {
  // Check if we are at a single-line comment indicator and
  // consume the comment.
  auto in_comment = false;
  bool is_beginning = _b.pos().row == 0;

  // If we are on the first line, and equals "#!" (shebang)
  if (is_beginning && (_b.peek(0) == 0x23 && _b.peek(1) == 0x21)) {
    in_comment = true;
    _b.pop();
    _b.pop();
  }

  if (_b.peek(0) == 0x2f && _b.peek(1) == 0x2f) {  // '//'
    in_comment = true;
    _b.pop();
    _b.pop();
  }

  if (in_comment) {
    // Consume until we reach the end-of-line
    while (_b.peek() != 0x0a) _b.pop();
  }

  return in_comment;
}

void Tokenizer::_consume_number(std::stringstream& ss, unsigned base) {
  for (unsigned idx = 0; ; ++idx) {
    // Peek at the next digit
    auto ch = _b.peek();

    // Check if this is a valid digit (for our base)
    if (idx != 0 && ch == '_') {
      _b.pop();
      continue;
    } else if (base == 16) {
      if (!(in_range(ch, '0', '9') ||
            in_range(ch, 'a', 'f') ||
            in_range(ch, 'A', 'F'))) {
        break;
      }
    } else if (!in_range(ch, '0', '0' + (base - 1))) {
      break;
    }

    // Push it into the buffer
    ss << static_cast<char>(ch);

    // Advance the input buffer
    _b.pop();
  }
}

auto Tokenizer::_scan_numeric() -> Ref<Token> {
  // Peek ahead and see if we /are/ a digit (and stop now if we aren't)
  if (!in_range(_b.peek(), '0', '9')) return nullptr;

  // Initialize the text buffer.
  std::stringstream text;

  // Store the initial position.
  auto begin = _b.pos();

  // Declare a var to store the inferred type and base.
  auto type = Token::Type::Integer;
  auto base = 10;

  // Determine the base of the number
  if (_b.peek(0) == '0') {
    auto ch = _b.peek(1);
    auto prefix = true;
    if (ch == 'x' || ch == 'X') {
      // Hexadecimal
      base = 16;
    } else if (ch == 'b' || ch == 'B') {
      // Binary
      base = 2;
    } else if (ch == 'o' || ch == 'O') {
      // Octal
      base = 8;
    } else {
      // decimal
      prefix = false;
    }

    // Consume the base prefix (if present)
    if (prefix) {
      _b.pop();
      _b.pop();
    }
  }

  // Consume the (first part of the) number
  _consume_number(text, base);

  // We are no longer a number (within our range)
  if (base == 10) {
    if (_b.peek() == '.' && in_range(_b.peek(1), '0', '9')) {
      // We have at least '.#' to go; we can be a float
      type = Token::Type::Float;

      // Push the `.` into the buffer
      text << static_cast<char>(_b.pop());

      // Consume the fractional part of the number
      _consume_number(text, base);
    }

    // We could continue into a scientific notation
    // with `[eE][+-]?[0-9]`
    auto p0 = _b.peek(0);
    auto p1 = _b.peek(1);
    auto p2 = _b.peek(2);
    if ((p0 == 0x45 || p0 == 0x65)
          && (in_range(p1, '0', '9')
            || ((p1 == 0x2b || p1 == 0x2d) && in_range(p2, '0', '9')))) {
      // We have at least [eE][+-]#
      type = Token::Type::Float;

      // Push the first two characters.
      text << static_cast<char>(_b.pop());
      text << static_cast<char>(_b.pop());

      // Consume the expected number (again).
      _consume_number(text, base);
    }
  }

  // Construct the string
  auto str = text.str();
  if (type == Token::Type::Integer && base != 10) {
    // Mush the base to 10
    mpz_t value;
    mpz_init_set_str(value, str.c_str(), base);
    auto bytes = mpz_get_str(nullptr, 10, value);
    str = std::string(bytes);
    free(bytes);
    mpz_clear(value);
  }

  // Construct and return the token.
  auto span = Span(_filename, begin, _b.pos());
  if (type == Token::Type::Float) {
    return new FloatToken(span, str);
  } else {
    return new IntegerToken(span, str);
  }
}

auto Tokenizer::_scan_string() -> Ref<Token> {
  auto begin = _b.pos();

  // Peek and see if we are a double-quoted string
  auto p0 = _b.peek(0);
  if (p0 != '\"') {
    // Not a string
    return nullptr;
  }

  // Drop the quote character(s)
  _b.pop();

  // Declare a buffer for bytes
  std::vector<char> bytes;

  // Iterate through the string token
  // Keep in mind escape sequences; the token text stores the
  // unescaped version (so the rest of the compiler can ignore
  // escapes)
  auto in_escape = false;
  auto in_byte_escape = false;

  // TODO(mehcode): auto in_unicode_escape = false;
  for (;;) {
    if (in_escape) {
      // Check if we have an extension control character.
      auto byte = _b.pop();
      switch (byte) {
        case 0x58:  // `X`
        case 0x78:  // `x`
          in_byte_escape = true;
          // fallthrough

        case 0x5c:  // `\`
        case 0x27:  // `'`
        case 0x22:  // `"`
        case 0x61:  // `a`
        case 0x62:  // `b`
        case 0x66:  // `f`
        case 0x6e:  // `n`
        case 0x72:  // `r`
        case 0x74:  // `t`
        case 0x76:  // `v`
          bytes.push_back(byte);
          break;

        default:
          Log::get().error(Span(_filename, _b.pos() - 2, _b.pos()),
            "unknown character escape: %c", byte);

          break;
      }

      // No longer in an escape sequence.
      in_escape = false;
    } else if (in_byte_escape) {
      bytes.push_back(_b.pop());
      bytes.push_back(_b.pop());

      // No longer in an byte escape sequence.
      in_byte_escape = false;
    } else {
      auto byte = _b.pop();

      if (byte == 0x5c) {
        in_escape = true;
        bytes.push_back(byte);
      } else if (byte == 0x22) {
        // Found the matching double-quote; we're done with the string
        break;
      } else {
        bytes.push_back(byte);
      }
    }
  }

  auto span = Span(_filename, begin, _b.pos());
  return new StringToken(span, std::string(bytes.data(), bytes.size()));
}

auto Tokenizer::_scan_punctuator() -> Ref<Token> {
  // Fill a vector with 3 characters
  vector<uint32_t> test;
  for (unsigned index = 0; index < 3; ++index) {
    test.push_back(_b.peek(index));
  }

  // Iterate backwards and test in order of largest first
  Ref<Token> result = nullptr;
  unsigned index;
  for (index = 3; index > 0; --index) {
    auto rec = punctuators.find(test);
    if (rec != punctuators.end()) {
      result = new PunctuatorToken(
        rec->second, Span(_filename, _b.pos(), index));

      break;
    }

    test.pop_back();
  }

  // If we matched a token; bump the matched characters
  for ( ; result && index > 0; --index) _b.pop();

  return result;
}

auto Tokenizer::_scan_identifier() -> Ref<Token> {
  // REF: http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1518.htm

  // An identifier can contain ..
  static vector<vector<uint32_t>> can_contain = {
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
  static vector<vector<uint32_t>> not_start_with = {
    {0x30, 0x39},
    {0x0300, 0x036F},
    {0x1DC0, 0x1DFF},
    {0x20D0, 0x20FF},
    {0xFE20, 0xFE2F},
  };

  // Mark our current position
  auto begin = _b.pos();

  // Build our UTF8 identifier
  vector<std::uint8_t> bytes;
  unsigned count = 0;

  for (;;) {
    // Peek the next UTF-32 character
    auto ch = _b.peek(count);
    if (ch == 0) { break; }

    // Is this one of our "can-contain" characters
    if (!in_ranges(ch, can_contain)) {
      // No.. tough luck
      break;
    }

    // Are we at the beginning and is this one of our "must-not-start-with"
    // characters
    if (count == 0 && in_ranges(ch, not_start_with)) {
      // Yep.. tough luck
      break;
    }

    // Increment total counter and append our character
    count += 1;
    utf8::append(ch, std::back_inserter(bytes));
  }

  if (count == 0) {
    // Got nothing
    return nullptr;
  }

  // We found something.. pop the consumed bytes
  for (unsigned i = 0; i < count; ++i) _b.pop();

  // Make a string out of it
  auto text = std::string(reinterpret_cast<char*>(bytes.data()), bytes.size());
  auto span = Span(_filename, begin, _b.pos());

  // Check for a match against a keyword
  auto kw = keywords.find(text);
  if (kw != keywords.end()) {
    // Found a keyword; create keyword token
    return new KeywordToken(kw->second, span);
  }

  // Couldn't match a keyword; return as identifier
  return new IdentifierToken(span, text);
}

}  // namespace arrow
