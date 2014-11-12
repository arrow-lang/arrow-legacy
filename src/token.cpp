#include "arrow/token.hpp"

using namespace arrow;

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

IntegerToken::IntegerToken(unsigned base, const std::string& text, Span span)
  : Token(Type::Integer, span), base(base), text(text) {
}

IntegerToken::IntegerToken(unsigned base, std::string&& text, Span span)
  : Token(Type::Integer, span), base(base), text(text) {
}

FloatToken::FloatToken(const std::string& text, Span span)
  : Token(Type::Float, span), text(text) {
}

FloatToken::FloatToken(std::string&& text, Span span)
  : Token(Type::Float, span), text(text) {
}
