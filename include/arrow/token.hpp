#ifndef ARROW_TOKEN_H
#define ARROW_TOKEN_H 1

#include "arrow/span.hpp"

namespace arrow {

  enum class Type {
    /// An 'unknown' token type
    Unknown,

    /// The end-of-stream condition [no-value]
    End,

    /// An number with no fractional part [value]
    Integer,

    /// A number with a fraction part expressed as a floating-point [value]
    Float,

    /// Plus `+`
    /// Binary addition
    Plus,

    /// Minus `-`
    /// Unary integral negation
    /// Binary subtraction
    Minus,

    /// Left-slash `/`
    /// Binary floating-point division
    LSlash,

    /// Left-slash > left-slash `//`
    /// Binary integral division
    LSlash_LSlash,

    /// Percent-sign `%`
    /// Binary modulo
    Percent,

    /// Plus > equals `+=`
    /// Compound assignment with addition
    Plus_Equals,

    /// Minus > equals `-=`
    /// Compound assignment with subtraction
    Minus_Equals,

    /// Left-slash > equals `/=`
    /// Compound assignment with floating-point division
    LSlash_Equals,

    /// Left-slash > left-slash > equals `//=`
    /// Compound assignment with integral division
    LSlash_LSlash_Equals,

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

  };

  extern std::string to_string(Type type) noexcept;

  struct Token {
    Token(Type type, Span span);

    Type type;
    Span span;
  };

  struct IntegerToken : Token {
    IntegerToken(unsigned base, const std::string& text, Span span);
    IntegerToken(unsigned base, std::string&& text, Span span);

    unsigned base;
    std::string text;
  };

  struct FloatToken : Token {
    FloatToken(const std::string& text, Span span);
    FloatToken(std::string&& text, Span span);

    std::string text;
  };

}

#endif // ARROW_TOKEN_H
