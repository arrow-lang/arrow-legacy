// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/command/read.hpp"
#include "arrow/buffer.hpp"

#include "utfcpp/utf8.h"

namespace po = boost::program_options;

namespace arrow {
namespace command {

void Read::add_options(po::options_description& desc) {
  desc.add_options()
    ("limit", po::value<unsigned>())
    ("offset", po::value<unsigned>());
}

int Read::run(po::variables_map& vm) {
  if (!vm.count("input-file")) {
    // TODO: Use 'stdin'
    return EXIT_FAILURE;
  }

  // Get limit and offset (if given)
  unsigned offset = vm.count("offset") ? vm["offset"].as<unsigned>() : 0;
  unsigned limit = vm.count("limit") ? vm["limit"].as<unsigned>() : 0;

  // Build and bind a buffer to the input file
  arrow::Buffer buffer{vm["input-file"].as<std::string>()};

  // Skip <offset> characters
  for (unsigned i = offset; !buffer.empty() && i > 0; --i) buffer.pop();

  // Enumerate until the end-of-file
  unsigned idx = offset;
  while (!buffer.empty() && (limit == 0 || (idx - offset) < limit)) {
    auto ch = buffer.pop();

    std::printf("\x1b[0;34m%8x  ", idx);

    std::printf("\x1b[0;37m");
    std::printf("%02x ", (ch & 0xff000000) >> 24);
    std::printf("%02x ", (ch & 0x00ff0000) >> 16);
    std::printf("%02x ", (ch & 0x0000ff00) >>  8);
    std::printf("%02x  ",  (ch & 0x000000ff));

    if (ch >= 32) {
      std::vector<unsigned char> utf8result;
      std::uint32_t utf32string[] = {ch, 0};
      utf8::utf32to8(utf32string, utf32string + 1, back_inserter(utf8result));
      utf8result.push_back(0);

      std::printf("\x1b[1;37m%s", utf8result.data());
    }

    std::printf("\x1b[0m\n");

    idx += 1;
  }

  return 0;
}

}  // namespace command {
}  // namespace arrow {
