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

    case Type::And:
      return "`and`";

    case Type::Or:
      return "`or`";

    case Type::Not:
      return "`not`";

    case Type::Xor:
      return "`xor`";

    case Type::Def:
      return "`def`";

    case Type::Let:
      return "`let`";

    case Type::Mut:
      return "`mut`";

    case Type::True:
      return "`true`";

    case Type::False:
      return "`false`";

    case Type::If:
      return "`if`";

    case Type::While:
      return "`while`";

    case Type::Break:
      return "`break`";

    case Type::Continue:
      return "`continue`";

    case Type::Return:
      return "`return`";

    case Type::Global:
      return "`global`";

    case Type::Arrow:
      return "`->`";

    case Type::FatArrow:
      return "`=>`";

    case Type::Comma:
      return "`,`";

    case Type::Plus:
      return "`+`";

    case Type::Minus:
      return "`-`";

    case Type::Slash:
      return "`/`";

    case Type::Asterisk:
      return "`*`";

    case Type::Percent:
      return "`%`";

    case Type::Plus_Equals:
      return "`+=`";

    case Type::Minus_Equals:
      return "`-=`";

    case Type::Slash_Equals:
      return "`/=`";

    case Type::Asterisk_Equals:
      return "`*=`";

    case Type::Percent_Equals:
      return "`%=`";

    case Type::Ampersand:
      return "`&`";

    case Type::Ampersand_Equals:
      return "`&=`";

    case Type::Pipe:
      return "`|`";

    case Type::Pipe_Equals:
      return "`|=`";

    case Type::Caret:
      return "`^`";

    case Type::Caret_Equals:
      return "`^=`";

    case Type::ExclamationMark:
      return "`!`";

    case Type::Equals:
      return "`=`";

    case Type::Equals_Equals:
      return "`==`";

    case Type::ExclamationMark_Equals:
      return "`!=`";

    case Type::GreaterThan:
      return "`>`";

    case Type::GreaterThan_Equals:
      return "`>=`";

    case Type::LessThan:
      return "`<`";

    case Type::LessThan_Equals:
      return "`<=`";

    case Type::Colon:
      return "`:`";

    case Type::Semicolon:
      return "`;`";

    case Type::LeftBrace:
      return "`{`";

    case Type::RightBrace:
      return "`}`";

    case Type::LeftBracket:
      return "`[`";

    case Type::RightBracket:
      return "`]`";

    case Type::LeftParenthesis:
      return "`(`";

    case Type::RightParenthesis:
      return "`)`";

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
