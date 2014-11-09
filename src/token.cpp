#include "arrow/token.hpp"

using arrow::Type;
using arrow::Token;

std::string arrow::to_string(Type type) noexcept {
  switch (type) {
    case Type::End:
      return "end";

    case Type::Integer:
      return "integer";

    case Type::Float:
      return "float";

    default:
      return "unknown";
  }
}

Token::Token(Type type, Span span)
  : type(type), span(span) {
}
