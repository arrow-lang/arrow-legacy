#ifndef ARROW_TOKENIZER_H
#define ARROW_TOKENIZER_H 1

#include <queue>
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
    std::shared_ptr<Token> peek(unsigned offset = 0);

    /// Get (consume) the next token in the input stream.
    std::shared_ptr<Token> pop();

    /// Get the filename that was used to bind the file handle.
    const std::string& filename() const noexcept {
      return _filename;
    }

  private:
    void _push();

    Position _pos() const;
    std::uint32_t _buffer_next();

    std::shared_ptr<Token> _make_token(
      Type type, Position begin, Position end) const;

    std::shared_ptr<Token> _scan_numeric();
    std::shared_ptr<Token> _scan_punctuator();
    std::shared_ptr<Token> _scan_identifier();

    bool _is_eol(bool consume = false);

    std::string _filename;
    Buffer _buffer;
    unsigned _row;
    unsigned _column;
    std::deque<std::shared_ptr<Token>> _queue;

  };

}

#endif // ARROW_TOKENIZER_H
