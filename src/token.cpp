// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <string>
#include <map>

#include "arrow/token.hpp"

#define IMPL(N) \
  N::~N() noexcept { }

namespace arrow {

static std::map<Token::Type, const char*> token_str_map = {
  {Token::Type::End,               "end"},
  {Token::Type::Identifier,        "identifier"},
  {Token::Type::Integer,           "integer"},
  {Token::Type::Float,             "float"},
  {Token::Type::String,            "string"},

  /// Keywords
  {Token::Type::None,              "`None`"},
  {Token::Type::TypeOf,            "`type`"},
  {Token::Type::And,               "`and`"},
  {Token::Type::Or,                "`or`"},
  {Token::Type::Not,               "`not`"},
  {Token::Type::Let,               "`let`"},
  {Token::Type::Mutable,           "`mutable`"},
  {Token::Type::Def,               "`def`"},
  {Token::Type::Extern,            "`extern`"},
  {Token::Type::Export,            "`export`"},
  {Token::Type::Import,            "`import`"},
  {Token::Type::From,              "`from`"},
  {Token::Type::True,              "`true`"},
  {Token::Type::False,             "`false`"},
  {Token::Type::If,                "`if`"},
  {Token::Type::Unless,            "`unless`"},
  {Token::Type::Else,              "`else`"},
  {Token::Type::Loop,              "`loop`"},
  {Token::Type::While,             "`while`"},
  {Token::Type::Until,             "`until`"},
  {Token::Type::Break,             "`break`"},
  {Token::Type::Continue,          "`continue`"},
  {Token::Type::Return,            "`return`"},
  {Token::Type::Global,            "`global`"},
  {Token::Type::As,                "`as`"},
  {Token::Type::Struct,            "`struct`"},
  {Token::Type::Underscore,        "`_`"},
  {Token::Type::Use,               "`use`"},

  /// Punctuators (1-character)
  {Token::Type::Plus,              "`+`"},
  {Token::Type::Minus,             "`-`"},
  {Token::Type::Slash,             "`/`"},
  {Token::Type::Asterisk,          "`*`"},
  {Token::Type::Percent,           "`%`"},
  {Token::Type::Ampersand,         "`&`"},
  {Token::Type::Pipe,              "`|`"},
  {Token::Type::Caret,             "`^`"},
  {Token::Type::ExclamationMark,   "`!`"},
  {Token::Type::Equals,            "`=`"},
  {Token::Type::LessThan,          "`<`"},
  {Token::Type::GreaterThan,       "`>`"},
  {Token::Type::Period,            "`.`"},
  {Token::Type::Colon,             "`:`"},
  {Token::Type::Semicolon,         "`;`"},
  {Token::Type::LeftBrace,         "`{`"},
  {Token::Type::RightBrace,        "`}`"},
  {Token::Type::LeftParenthesis,   "`(`"},
  {Token::Type::RightParenthesis,  "`)`"},
  {Token::Type::LeftBracket,       "`[`"},
  {Token::Type::RightBracket,      "`]`"},
  {Token::Type::Comma,             "`,`"},

  /// Punctuators (2-character)
  {Token::Type::Plus_Equals,              "`+=`"},
  {Token::Type::Minus_Equals,             "`-=`"},
  {Token::Type::Asterisk_Equals,          "`*=`"},
  {Token::Type::Slash_Equals,             "`/=`"},
  {Token::Type::Percent_Equals,           "`%=`"},
  {Token::Type::Ampersand_Equals,         "`&=`"},
  {Token::Type::Pipe_Equals,              "`|=`"},
  {Token::Type::Caret_Equals,             "`^=`"},
  {Token::Type::Equals_Equals,            "`==`"},
  {Token::Type::ExclamationMark_Equals,   "`!=`"},
  {Token::Type::GreaterThan_Equals,       "`>=`"},
  {Token::Type::LessThan_Equals,          "`<=`"},
  {Token::Type::Arrow,                    "`->`"},
  {Token::Type::FatArrow,                 "`=>`"},

  /// Punctuators (3-character)
  {Token::Type::Ellipsis,                 "`...`"},
};

IMPL(Token)
IMPL(FloatToken)
IMPL(IntegerToken)
IMPL(StringToken)
IMPL(IdentifierToken)
IMPL(PunctuatorToken)
IMPL(KeywordToken)

std::string to_string(Token::Type type) noexcept {
  return token_str_map[type];
}

std::string Token::to_string() const noexcept {
  switch (type) {
    case Type::End: return "end";
    default:
      // unreachable
      return "";
  }
}

std::string IntegerToken::to_string() const noexcept {
  return "integer: " + text;
}

std::string FloatToken::to_string() const noexcept {
  return "float: " + text;
}

std::string IdentifierToken::to_string() const noexcept {
  return "identifier: " + text;
}

std::string StringToken::to_string() const noexcept {
  // NOTE: We don't return the whole textual string; just saying its a
  //  string is good enough
  return "string";
}

std::string PunctuatorToken::to_string() const noexcept {
  switch (type) {
    /// Punctuators (1-character)
    case Type::Plus:              return "punctuator: +";
    case Type::Minus:             return "punctuator: -";
    case Type::Slash:             return "punctuator: /";
    case Type::Asterisk:          return "punctuator: *";
    case Type::Percent:           return "punctuator: %";
    case Type::Ampersand:         return "punctuator: &";
    case Type::Pipe:              return "punctuator: |";
    case Type::Caret:             return "punctuator: ^";
    case Type::ExclamationMark:   return "punctuator: !";
    case Type::Equals:            return "punctuator: =";
    case Type::LessThan:          return "punctuator: <";
    case Type::GreaterThan:       return "punctuator: >";
    case Type::Period:            return "punctuator: .";
    case Type::Colon:             return "punctuator: :";
    case Type::Semicolon:         return "punctuator: ;";
    case Type::LeftBrace:         return "punctuator: {";
    case Type::RightBrace:        return "punctuator: }";
    case Type::LeftParenthesis:   return "punctuator: (";
    case Type::RightParenthesis:  return "punctuator: )";
    case Type::LeftBracket:       return "punctuator: [";
    case Type::RightBracket:      return "punctuator: ]";
    case Type::Comma:             return "punctuator: ,";

    /// Punctuators (2-character)
    case Type::Plus_Equals:                return "punctuator: +=";
    case Type::Minus_Equals:               return "punctuator: -=";
    case Type::Asterisk_Equals:            return "punctuator: *=";
    case Type::Slash_Equals:               return "punctuator: /=";
    case Type::Percent_Equals:             return "punctuator: %=";
    case Type::Ampersand_Equals:           return "punctuator: &=";
    case Type::Pipe_Equals:                return "punctuator: |=";
    case Type::Caret_Equals:               return "punctuator: ^=";
    case Type::Equals_Equals:              return "punctuator: ==";
    case Type::ExclamationMark_Equals:     return "punctuator: !=";
    case Type::GreaterThan_Equals:         return "punctuator: >=";
    case Type::LessThan_Equals:            return "punctuator: <=";
    case Type::Arrow:                      return "punctuator: ->";
    case Type::FatArrow:                   return "punctuator: =>";

    /// Punctuators (3-character)
    case Type::Ellipsis:                   return "punctuator: ...";
    default:
      // unreachable
      return "";
  }
}

std::string KeywordToken::to_string() const noexcept {
  switch (type) {
    case Type::None:       return "keyword: None";
    case Type::TypeOf:     return "keyword: type";
    case Type::And:        return "keyword: and";
    case Type::Or:         return "keyword: or";
    case Type::Not:        return "keyword: not";
    case Type::Let:        return "keyword: let";
    case Type::Mutable:    return "keyword: mutable";
    case Type::Def:        return "keyword: def";
    case Type::Extern:     return "keyword: extern";
    case Type::Export:     return "keyword: export";
    case Type::Import:     return "keyword: import";
    case Type::From:       return "keyword: from";
    case Type::True:       return "keyword: true";
    case Type::False:      return "keyword: false";
    case Type::If:         return "keyword: if";
    case Type::Unless:     return "keyword: unless";
    case Type::Else:       return "keyword: else";
    case Type::Loop:       return "keyword: loop";
    case Type::While:      return "keyword: while";
    case Type::Until:      return "keyword: until";
    case Type::Break:      return "keyword: break";
    case Type::Continue:   return "keyword: continue";
    case Type::Return:     return "keyword: return";
    case Type::Global:     return "keyword: global";
    case Type::As:         return "keyword: as";
    case Type::Struct:     return "keyword: struct";
    case Type::Underscore: return "keyword: _";
    case Type::Use:        return "keyword: use";
    default:
      // unreachable
      return "";
  }
}

}  // namespace arrow
