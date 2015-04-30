// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <string>
#include "arrow/token.hpp"

using arrow::Token;
using arrow::TextToken;
using arrow::IntegerToken;
using arrow::FloatToken;
using arrow::IdentifierToken;
using arrow::CharacterToken;
using arrow::StringToken;

std::string arrow::to_string(Token::Type type) noexcept {
  switch (type) {
    case Token::Type::End:
      return "end";

    case Token::Type::Integer:
      return "integer";

    case Token::Type::Float:
      return "float";

    case Token::Type::Identifier:
      return "identifier";

    case Token::Type::Character:
      return "character";

    case Token::Type::String:
      return "string";

    case Token::Type::And:
      return "`and`";

    case Token::Type::Or:
      return "`or`";

    case Token::Type::Not:
      return "`not`";

    case Token::Type::Xor:
      return "`xor`";

    case Token::Type::Def:
      return "`def`";

    case Token::Type::Let:
      return "`let`";

    case Token::Type::Mut:
      return "`mut`";

    case Token::Type::True:
      return "`true`";

    case Token::Type::False:
      return "`false`";

    case Token::Type::If:
      return "`if`";

    case Token::Type::While:
      return "`while`";

    case Token::Type::Break:
      return "`break`";

    case Token::Type::Continue:
      return "`continue`";

    case Token::Type::Return:
      return "`return`";

    case Token::Type::Global:
      return "`global`";

    case Token::Type::Arrow:
      return "`->`";

    case Token::Type::FatArrow:
      return "`=>`";

    case Token::Type::Comma:
      return "`,`";

    case Token::Type::Plus:
      return "`+`";

    case Token::Type::Minus:
      return "`-`";

    case Token::Type::Slash:
      return "`/`";

    case Token::Type::Asterisk:
      return "`*`";

    case Token::Type::Percent:
      return "`%`";

    case Token::Type::Plus_Equals:
      return "`+=`";

    case Token::Type::Minus_Equals:
      return "`-=`";

    case Token::Type::Slash_Equals:
      return "`/=`";

    case Token::Type::Asterisk_Equals:
      return "`*=`";

    case Token::Type::Percent_Equals:
      return "`%=`";

    case Token::Type::Ampersand:
      return "`&`";

    case Token::Type::Ampersand_Equals:
      return "`&=`";

    case Token::Type::Pipe:
      return "`|`";

    case Token::Type::Pipe_Equals:
      return "`|=`";

    case Token::Type::Caret:
      return "`^`";

    case Token::Type::Caret_Equals:
      return "`^=`";

    case Token::Type::ExclamationMark:
      return "`!`";

    case Token::Type::Equals:
      return "`=`";

    case Token::Type::Equals_Equals:
      return "`==`";

    case Token::Type::ExclamationMark_Equals:
      return "`!=`";

    case Token::Type::GreaterThan:
      return "`>`";

    case Token::Type::GreaterThan_Equals:
      return "`>=`";

    case Token::Type::LessThan:
      return "`<`";

    case Token::Type::LessThan_Equals:
      return "`<=`";

    case Token::Type::Colon:
      return "`:`";

    case Token::Type::Semicolon:
      return "`;`";

    case Token::Type::LeftBrace:
      return "`{`";

    case Token::Type::RightBrace:
      return "`}`";

    case Token::Type::LeftBracket:
      return "`[`";

    case Token::Type::RightBracket:
      return "`]`";

    case Token::Type::LeftParenthesis:
      return "`(`";

    case Token::Type::RightParenthesis:
      return "`)`";

    default:
      return "unknown";
  }
}

Token::Token(Type type, Span span)
  : type(type), span(span) {
}

IntegerToken::IntegerToken(unsigned base, const std::string& text, Span span)
  : TextToken(Type::Integer, span, text), base(base) {
}

TextToken::TextToken(Type type, Span span, const std::string& text)
  : Token(type, span), text(text) {
}

FloatToken::FloatToken(const std::string& text, Span span)
  : TextToken(Type::Float, span, text) {
}

IdentifierToken::IdentifierToken(const std::string& text, Span span)
  : TextToken(Type::Identifier, span, text) {
}

CharacterToken::CharacterToken(std::uint32_t ch, Span span)
  : Token(Type::Character, span), character{ch} {
}

StringToken::StringToken(const std::vector<std::uint8_t>& bytes, Span span)
  : Token(Type::String, span), bytes(bytes) {
}
