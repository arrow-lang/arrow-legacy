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
  };

  extern std::string to_string(Type type) noexcept;

  struct Token {
    Token(Type type, Span span);

    Type type;
    Span span;
  };

}

#endif // ARROW_TOKEN_H
