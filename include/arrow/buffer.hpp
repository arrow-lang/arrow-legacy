#ifndef ARROW_BUFFER_H
#define ARROW_BUFFER_H 1

#include <memory>
#include <istream>
#include <string>
#include <deque>

namespace arrow {

  /// Maintains an N-lookahead buffer of bytes from an input stream.
  class Buffer {
  public:
    /// Constructs a `Buffer` bound to a file handle.
    Buffer(const std::string& filename);

    /// Peek (perserve) the next byte and test if we are at the end
    /// of the input stream.
    bool empty();

    /// Peek (perserve) the byte `offset` bytes away from the current position
    /// in the input stream.
    std::uint8_t peek(unsigned offset = 0);

    /// Pop (dispose) of the next byte in the input stream.
    void pop();

    /// Get (consume) the next byte in the input stream.
    std::uint8_t next();

  private:
    bool _push();

    std::unique_ptr<std::istream> _stream;
    std::deque<std::uint8_t> _queue;

  };

}

#endif // ARROW_BUFFER_H
