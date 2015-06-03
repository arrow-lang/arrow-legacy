// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_TOKEN_H
#define ARROW_TOKEN_H 1

#include <string>
#include <vector>
#include "arrow/span.hpp"

namespace arrow {

struct Token {
  enum class Type {
    /// An 'unknown' token type
    Unknown,

    /// The end-of-stream condition [no-value]
    End,

    /// A symbolic identifier
    Identifier,

    /// An number with no fractional part [value]
    Integer,

    /// A number with a fraction part expressed as a floating-point [value]
    Float,

    /// An UTF-8 character
    Character,

    /// An UTF-8 string
    String,

    /// And
    And,

    /// Or
    Or,

    /// Xor
    Xor,

    /// Not
    Not,

    /// Let
    Let,

    /// Mut
    Mut,

    /// Def
    Def,

    /// Extern
    Extern,

    /// Plus `+`
    /// Binary addition
    Plus,

    /// Minus `-`
    /// Unary integral negation
    /// Binary subtraction
    Minus,

    /// Slash `/`
    /// Binary floating-point division
    Slash,

    /// Asterisk `*`
    /// Binary multiplication or pointer derefernce
    Asterisk,

    /// Asterisk > equals `*=`
    /// Compound assignment with multiplication
    Asterisk_Equals,

    /// Percent-sign `%`
    /// Binary modulo
    Percent,

    /// Plus > equals `+=`
    /// Compound assignment with addition
    Plus_Equals,

    /// Minus > equals `-=`
    /// Compound assignment with subtraction
    Minus_Equals,

    /// Slash > equals `/=`
    /// Compound assignment with floating-point division
    Slash_Equals,

    /// Percent-sign > equals `%=`
    /// Compound assignment with modulo
    Percent_Equals,

    /// Ampersand `&`
    /// Unary address-of (unsafe operation)
    /// Binary bitwise-AND or logical-AND (depending on operand types)
    Ampersand,

    /// Amerspand > equals `&=`
    /// Compound assignment with bitwise-AND or logical-AND (
    ///   depending on operand types)
    Ampersand_Equals,

    /// Pipe `|`
    /// Binary bitwise-OR or logical-OR (depending on operand types)
    Pipe,

    /// Pipe > equals `|=`
    /// Compound assignment with bitwise-OR or logical-OR (
    ///   depending on operand types)
    Pipe_Equals,

    /// Caret `^`
    /// Binary bitwise-XOR or logical-XOR (depending on operand types)
    Caret,

    /// Caret > equals `^=`
    /// Compound assignment with bitwise-XOR or logical-XOR (
    ///   depending on operand types)
    Caret_Equals,

    /// Exclamation-mark `!`
    /// Unary bitwise-NOT or logical-NOT (depending on operand types)
    ExclamationMark,

    /// Equals `=`
    /// Assignment
    Equals,

    /// Equals > Equals `==`
    /// Binary equality relation
    Equals_Equals,

    /// Exclamation-mark > Equals `!=`
    /// Binary in-equality relation
    ExclamationMark_Equals,

    /// Greater-than `>`
    /// Binary greater-than relation
    GreaterThan,

    /// Greater-than > equals `>=`
    /// Binary greater-than-or-equal-to relation
    GreaterThan_Equals,

    /// Less-than `<`
    /// Binary less-than relation
    LessThan,

    /// Less-than > equals `<=`
    /// Binary less-than-or-equal-to relation
    LessThan_Equals,

    /// Colon `:`
    Colon,

    /// Semi-colon `;`
    Semicolon,

    /// Left-brace `{`
    LeftBrace,

    /// Right-brace `}`
    RightBrace,

    /// Left-parenethis `(`
    LeftParenthesis,

    /// Right-parenethis `)`
    RightParenthesis,

    /// Left-parenethis `[`
    LeftBracket,

    /// Right-parenethis `]`
    RightBracket,

    /// Arrow `->`
    Arrow,

    /// Fat arrow `=>`
    FatArrow,

    /// Comma `,`
    Comma,

    /// True
    True,

    /// False
    False,

    /// If
    If,

    /// Else
    Else,

    /// Loop
    Loop,

    /// While
    While,

    /// Break
    Break,

    /// Continue
    Continue,

    /// Return
    Return,

    /// Global
    Global
  };

  Token(Type type, Span span);

  Type type;
  Span span;
};

extern std::string to_string(Token::Type type) noexcept;

struct TextToken : Token {
  TextToken(Token::Type type, Span span, const std::string& text);

  std::string text;
};

struct IntegerToken : TextToken {
  IntegerToken(unsigned base, const std::string& text, Span span);

  unsigned base;
};

struct FloatToken : TextToken {
  FloatToken(const std::string& text, Span span);
};

struct IdentifierToken : TextToken {
  IdentifierToken(const std::string& text, Span span);
};

struct CharacterToken : Token {
  CharacterToken(std::uint32_t character, Span span);

  std::uint32_t character;
};

struct StringToken : Token {
  StringToken(const std::vector<std::uint8_t>& bytes, Span span);

  std::vector<std::uint8_t> bytes;
};

}  // namespace arrow

#endif  // ARROW_TOKEN_H
