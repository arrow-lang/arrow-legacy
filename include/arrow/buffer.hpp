// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_BUFFER_H
#define ARROW_BUFFER_H 1

#include <memory>
#include <istream>
#include <string>
#include <deque>

#include "arrow/span.hpp"

namespace arrow {

/// Maintains an N-lookahead buffer of UTF-32 from an input stream.
class Buffer {
 public:
  /// Constructs a `Buffer` bound to a file handle.
  explicit Buffer(const std::string& filename);

  /// Constructs a `Buffer` bound to an input stream.
  explicit Buffer(std::shared_ptr<std::istream> is);

  /// Peek (perserve) and test if we are at the end
  /// of the input stream.
  inline bool empty() {
    return (peek(0) == 0);
  }

  /// Peek (perserve) the character `offset` characters away from
  /// the current position in the input stream.
  std::uint32_t peek(unsigned offset = 0);

  /// Pop (dispose) of the next character in the input stream.
  std::uint32_t pop();

  /// Get the current position of the cursor.
  inline Position pos() const noexcept {
    return Position(_row, _column);
  }

 private:
  /// Read more characters from the bound input stream
  /// until we can fulfill the request.
  bool _read(unsigned count);

  std::shared_ptr<std::istream> _stream;

  // NOTE: We use a `deque` instead of a `queue` because while this is a
  //       FIFO buffer; we need to be able to peek with N-lookahead.
  std::deque<std::uint32_t> _queue;

  /// Current (row, column) position of the buffer in the input file.
  std::size_t _row = 0;
  std::size_t _column = 0;
};

}  // namespace arrow

#endif  // ARROW_BUFFER_H
