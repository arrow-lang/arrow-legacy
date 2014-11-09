#include <fstream>
#include "arrow/buffer.hpp"

using arrow::Buffer;

Buffer::Buffer(const std::string& filename)
  : _stream(new std::ifstream(filename, std::ios::in|std::ios::binary)) {
}

bool Buffer::_push() {
  uint8_t ch;
  this->_stream->read(reinterpret_cast<char*>(&ch), 1);
  if (this->_stream->gcount() == 1) {
    this->_queue.push_back(ch);
    return true;
  } else {
    return false;
  }
}

uint8_t Buffer::next() {
  if (this->_queue.size() == 0) {
    // Ask for another byte.
    if (!this->_push()) {
      // Reached end-of-stream.
      return 0;
    }
  }

  // Get (and consume) the next byte.
  auto front = this->_queue.front();
  this->_queue.pop_front();
  return front;
}

void Buffer::pop() {
  // Get and don't return
  this->next();
}

uint8_t Buffer::peek(unsigned offset) {
  // Ask for more bytes until we can fulfill the request
  while (this->_queue.size() <= offset) {
    if (!this->_push()) {
      // Reached end-of-stream.
      return 0;
    }
  }

  // Peek (and perserve) the `offset` byte.
  return this->_queue.at(offset);
}

bool Buffer::empty() {
  return this->peek(0) == 0;
}
