#include <fstream>
#include <iterator>
#include "arrow/buffer.hpp"
#include "utf8.h"

using arrow::Buffer;

Buffer::Buffer(const std::string& filename)
  : _stream(new std::ifstream(filename, std::ios::in|std::ios::binary)) {
}

bool Buffer::_push() {
  std::istreambuf_iterator<char> beg(*_stream);
  std::istreambuf_iterator<char> end;
  if (beg == end) {
    return false;
  }

  auto ch = utf8::next(beg, end);
  if (ch == 0) {
    return false;
  } else {
    _queue.push_back(ch);
    return true;
  }
}

std::uint32_t Buffer::pop() {
  if (_queue.size() == 0) {
    // Ask for another character.
    if (!_push()) {
      // Reached end-of-stream.
      return 0;
    }
  }

  // Get (and consume) the next byte.
  auto front = _queue.front();
  _queue.pop_front();
  return front;
}

std::uint32_t Buffer::peek(unsigned offset) {
  // Ask for more characters until we can fulfill the request
  while (_queue.size() <= offset) {
    if (!_push()) {
      // Reached end-of-stream.
      return 0;
    }
  }

  // Peek (and perserve) the `offset` character.
  return _queue.at(offset);
}

bool Buffer::empty() {
  return peek(0) == 0;
}
