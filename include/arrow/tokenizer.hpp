#ifndef ARROW_TOKENIZER_H
#define ARROW_TOKENIZER_H 1

#include "arrow/token.hpp"
#include "arrow/buffer.hpp"

namespace arrow {

  class Tokenizer {
  public:
    /// Constructs a `tokenizer` bound to a file handle [recommended].
    Tokenizer(const std::string& filename);

    /// Peek (perserve) the next token and test if we are at the end
    /// of the input stream.
    bool empty();

    /// Peek (perserve) the token `offset` tokens away from the current position
    /// in the input stream.
    Token peek(unsigned offset = 0);

    /// Get (consume) the next token in the input stream.
    Token next();

  private:
    Buffer _buffer;
    std::string _filename;

  };

}

#endif // ARROW_TOKENIZER_H
