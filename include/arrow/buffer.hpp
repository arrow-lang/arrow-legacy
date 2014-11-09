#ifndef ARROW_BUFFER_H
#define ARROW_BUFFER_H 1

#include <memory>
#include <istream>
#include <string>

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

    /// Get (consume) the next byte in the input stream.
    std::uint8_t next();

  private:
    std::unique_ptr<std::istream> _stream;

  };

}

#endif // ARROW_BUFFER_H
