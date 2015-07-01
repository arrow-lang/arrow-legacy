// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_TOKEN_H
#define ARROW_TOKEN_H 1

#include <string>

#include "arrow/span.hpp"

namespace arrow {

struct Token {
  enum class Type {
    /// The end-of-stream condition.
    End,

    /// A UTF-8 encoded symbolic identifier.
    Identifier,

    /// An decimal number (with no fractional part).
    Integer,

    /// A decimal number with a fractional part.
    Float,

    /// A UTF-8 encoded sequence of characters; string.
    String,

    /// Keywords (used)
    None,       // None
    TypeOf,     // type
    And,        // and
    Or,         // or
    Not,        // not
    Let,        // let
    Mutable,    // mutable
    Def,        // def
    Extern,     // extern
    Export,     // export
    Import,     // import
    From,       // from
    True,       // true
    False,      // false
    If,         // if
    Unless,     // unless
    Else,       // else
    Loop,       // loop
    While,      // while
    Until,      // until
    Break,      // break
    Continue,   // continue
    Return,     // return
    Global,     // global
    As,         // as
    Struct,     // struct
    Underscore, // _

    /// Keywords (reserved)
    // For,        // for
    // Do,         // do
    // Enum,       // enum
    // Class,      // class
    // Macro,      // macro
    // Public,     // public
    // Private,    // private
    // Static,     // static
    // Self,       // self
    // Super,      // super
    // Trait,      // trait
    // Unsafe,     // unsafe
    // Use,        // use
    // Where,      // where
    // Yield,      // yield
    // Default,    // default
    // Union,      // union
    // Variant,    // variant
    // Match,      // match
    // Otherwise,  // otherwise
    // Module,     // module
    // Extends,    // extends
    // Debugger,   // debugger
    // In,         // in
    // Is,         // is
    // Of,         // of
    // Final,      // final
    // With,       // with
    // Implement,  // implement

    /// Punctuators (1-character)
    Plus,               // +
    Minus,              // -
    Slash,              // /
    Asterisk,           // *
    Percent,            // %
    Ampersand,          // &
    Pipe,               // |
    Caret,              // ^
    ExclamationMark,    // !
    Equals,             // =
    LessThan,           // <
    GreaterThan,        // >
    Period,             // .
    Colon,              // :
    Semicolon,          // ;
    LeftBrace,          // {
    RightBrace,         // }
    LeftParenthesis,    // (
    RightParenthesis,   // )
    LeftBracket,        // [
    RightBracket,       // ]
    Comma,              // ,

    /// Punctuators (2-character)
    Plus_Equals,                // +=
    Minus_Equals,               // -=
    Asterisk_Equals,            // *=
    Slash_Equals,               // /=
    Percent_Equals,             // %=
    Ampersand_Equals,           // &=
    Pipe_Equals,                // |=
    Caret_Equals,               // ^=
    Equals_Equals,              // ==
    ExclamationMark_Equals,     // !=
    GreaterThan_Equals,         // >=
    LessThan_Equals,            // <=
    Arrow,                      // ->
    FatArrow,                   // =>

    /// Punctuators (3-character)
    Ellipsis,                   // ...
  };

  inline Token(Type type, Span span) : type(type), span(span) {
  }

  virtual ~Token() noexcept;

  virtual std::string to_string() const noexcept;

  Type type;
  Span span;
};

extern std::string to_string(Token::Type type) noexcept;

#define DEF_TEXTUAL(T) \
  struct T##Token : Token { \
    virtual ~T##Token() noexcept; \
    virtual std::string to_string() const noexcept; \
    inline T##Token(Span span, std::string text) \
      : Token(Type::T, span), text(text) { \
    } \
    std::string text; \
  }

DEF_TEXTUAL(Integer);
DEF_TEXTUAL(Float);
DEF_TEXTUAL(String);
DEF_TEXTUAL(Identifier);

#undef DEF_TEXTUAL

#define DEF(N) \
  struct N##Token : Token { \
    virtual ~N##Token() noexcept; \
    virtual std::string to_string() const noexcept; \
    using Token::Token; \
  }

DEF(Punctuator);
DEF(Keyword);

#undef DEF

}  // namespace arrow

#endif  // ARROW_TOKEN_H
