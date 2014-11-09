#include <fstream>
#include "arrow/buffer.hpp"

using arrow::Buffer;

Buffer::Buffer(const std::string& filename)
  : _stream(new std::ifstream(filename, std::ios::in|std::ios::binary)) {
}

uint8_t Buffer::next() {
  uint8_t ch;
  this->_stream->read(reinterpret_cast<char*>(&ch), 1);
  return (this->_stream->gcount() == 1) ? ch : 0;
}
