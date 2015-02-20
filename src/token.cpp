#include "arrow/token.hpp"

using namespace arrow;

std::string arrow::to_string(Type type) noexcept
{
  switch (type) {
    case Type::End:
      return "end";

    case Type::Integer:
      return "integer";

    case Type::Float:
      return "float";

    case Type::Identifier:
      return "identifier";

    case Type::Plus:
      return "punctuator: +";

    case Type::Minus:
      return "punctuator: -";

    case Type::LSlash:
      return "punctuator: /";

    case Type::LSlash_LSlash:
      return "punctuator: //";

    case Type::Asterisk:
      return "punctuator: *";

    case Type::Percent:
      return "punctuator: %";

    case Type::Plus_Equals:
      return "punctuator: +=";

    case Type::Minus_Equals:
      return "punctuator: -=";

    case Type::LSlash_Equals:
      return "punctuator: /=";

    case Type::LSlash_LSlash_Equals:
      return "punctuator: //=";

    case Type::Asterisk_Equals:
      return "punctuator: *=";

    case Type::Percent_Equals:
      return "punctuator: %=";

    case Type::Ampersand:
      return "punctuator: &";

    case Type::Ampersand_Equals:
      return "punctuator: &=";

    case Type::Pipe:
      return "punctuator: |";

    case Type::Pipe_Equals:
      return "punctuator: |=";

    case Type::Caret:
      return "punctuator: ^";

    case Type::Caret_Equals:
      return "punctuator: ^=";

    case Type::ExclamationMark:
      return "punctuator: !";

    case Type::Equals:
      return "punctuator: =";

    case Type::Equals_Equals:
      return "punctuator: ==";

    case Type::ExclamationMark_Equals:
      return "punctuator: !=";

    case Type::GreaterThan:
      return "punctuator: >";

    case Type::GreaterThan_Equals:
      return "punctuator: >=";

    case Type::LessThan:
      return "punctuator: <";

    case Type::LessThan_Equals:
      return "punctuator: <=";

    case Type::Colon:
      return "punctuator: :";

    case Type::Semicolon:
      return "punctuator: ;";

    case Type::LeftBrace:
      return "punctuator: {";

    case Type::RightBrace:
      return "punctuator: }";

    case Type::LeftBracket:
      return "punctuator: [";

    case Type::RightBracket:
      return "punctuator: ]";

    case Type::LeftParenthesis:
      return "punctuator: (";

    case Type::RightParenthesis:
      return "punctuator: )";

    default:
      return "unknown";
  }
}

Token::Token(Type type, Span span)
  : type(type), span(span)
{
}

IntegerToken::IntegerToken(unsigned base, const std::string& text, Span span)
  : Token(Type::Integer, span), text(text), base(base)
{
}

IntegerToken::IntegerToken(unsigned base, std::string&& text, Span span)
  : Token(Type::Integer, span), text(text), base(base)
{
}

FloatToken::FloatToken(const std::string& text, Span span)
  : Token(Type::Float, span), text(text)
{
}

FloatToken::FloatToken(std::string&& text, Span span)
  : Token(Type::Float, span), text(text)
{
}

IdentifierToken::IdentifierToken(const std::string& text, Span span)
  : Token(Type::Identifier, span), text(text)
{
}

IdentifierToken::IdentifierToken(std::string&& text, Span span)
  : Token(Type::Identifier, span), text(text)
{
}
