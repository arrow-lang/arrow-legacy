// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_TOKENIZER_H
#define ARROW_TOKENIZER_H 1

#include <string>
#include <istream>
#include <sstream>
#include <queue>
#include <deque>
#include "arrow/ref.hpp"
#include "arrow/token.hpp"
#include "arrow/buffer.hpp"

namespace arrow {

class Tokenizer {
 public:
  /// Constructs a `tokenizer` bound to the input stream.
  Tokenizer(std::shared_ptr<std::istream> is, const std::string& filename);

  /// Peek (perserve) the next token and test if we are at the end
  /// of the input stream.
  bool empty() {
    return peek()->type == Token::Type::End;
  }

  /// Peek (perserve) the token `offset` tokens away from the current position
  /// in the input stream.
  Ref<Token> peek(unsigned offset = 0);

  /// Get (consume) the next token in the input stream.
  Ref<Token> pop();

  /// Get the filename.
  const std::string& filename() const noexcept { return _filename; }

 private:
  bool _read(std::size_t offset);

  bool _consume_line_comment();
  void _consume_number(std::stringstream& ss, unsigned base);

  Ref<Token> _scan_numeric();
  Ref<Token> _scan_punctuator();
  Ref<Token> _scan_identifier();
  Ref<Token> _scan_string();

  const std::string& _filename;
  std::deque<Ref<Token>> _queue;
  Buffer _b;
  std::vector<std::function<Ref<Token>()>> _scanners;
};

}  // namespace arrow

#endif  // ARROW_TOKENIZER_H
