// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <vector>
#include <string>

#include "arrow/command/read.hpp"
#include "arrow/buffer.hpp"

#include "utfcpp/utf8.h"

namespace po = boost::program_options;

namespace arrow {
namespace command {

int Read::run(std::shared_ptr<std::istream> is, const po::variables_map&) {
  // Build and bind a buffer to the input file
  arrow::Buffer buffer{is};

  // Enumerate until the end-of-file
  while (!buffer.empty()) {
    std::printf(" %-10s  ", buffer.pos().to_string().c_str());
    auto ch = buffer.pop();

    std::printf("%02x ", (ch & 0xff000000) >> 24);
    std::printf("%02x ", (ch & 0x00ff0000) >> 16);
    std::printf("%02x ", (ch & 0x0000ff00) >>  8);
    std::printf("%02x  ",  (ch & 0x000000ff));

    if (ch >= 32) {
      std::vector<unsigned char> utf8result;
      std::uint32_t utf32string[] = {ch, 0};
      utf8::utf32to8(utf32string, utf32string + 1, back_inserter(utf8result));
      utf8result.push_back(0);

      std::printf("%s", utf8result.data());
    }

    std::printf("\n");
  }

  return 0;
}

}  // namespace command
}  // namespace arrow
