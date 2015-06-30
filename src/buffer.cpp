// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <string>
#include <fstream>

#include "arrow/buffer.hpp"
#include "arrow/log.hpp"

#include "utfcpp/utf8.h"

namespace arrow {

Buffer::Buffer(std::shared_ptr<std::istream> is) : _stream(is), _queue() {
}

Buffer::Buffer(const std::string& filename) : _stream(nullptr), _queue() {
  // Open the passed file ..
  auto handle = new std::ifstream(filename, std::ios::in | std::ios::binary);
  if (!handle->is_open()) {
    // Couldn't open the file; hundred things could be wrong here
    Log::get().error(
      "couldn't read \"%s\"; couldn't open path as file", filename.c_str());

    return;
  }

  // Bind the now-open file stream as our input stream
  _stream.reset(handle);
}

std::uint32_t Buffer::pop() {
  if (!_read(1)) return 0;

  // Get (and consume) the next byte.
  // NOTE: Why can't C++ be sane and have `.pop` return a value
  auto ch = _queue.front();
  _queue.pop_front();

  // Advance the column position
  _column += 1;

  // If at a newline; reset the column position and advance the row position
  if (ch == 0x0a) {
    _column = 0;
    _row += 1;
  }

  return ch;
}

std::uint32_t Buffer::peek(unsigned offset) {
  if (!_read(offset + 1)) return 0;

  // Peek (and perserve) the `offset` character.
  return _queue.at(offset);
}

bool Buffer::_read(unsigned count) {
  // If we have enough characters in the queue; just return
  if (_queue.size() >= count) return true;

  std::istreambuf_iterator<char> end;
  unsigned needed = count - _queue.size();
  for (; needed > 0; --needed) {
    std::istreambuf_iterator<char> beg(*_stream);
    if (beg == end) {
      // Hit the end-of-file
      return false;
    }

    auto ch = utf8::next(beg, end);

    // Normalize line ending; only return 'Line Feed' to signal EOL
    if (ch == 0x0d) {  // Carriage Return
      ch = 0x0a;  // Interpret us as a Line Feed
      // Check if the next char is 'Carriage Return' and if so, drop it
      if (beg != end) {
        auto ch2 = utf8::next(beg, end);
        if (ch2 != 0x0a) {  // CR + LF or just CR
          _stream->seekg(-1, std::ios::cur);
        }
      }
    }

    _queue.push_back(ch);
  }

  return true;
}

}  // namespace arrow
