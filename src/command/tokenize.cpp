// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <vector>
#include <string>

#include "arrow/command/tokenize.hpp"
#include "arrow/tokenizer.hpp"
#include "arrow/log.hpp"

namespace po = boost::program_options;

namespace arrow {
namespace command {

int Tokenize::run(
  std::shared_ptr<std::istream> is,
  const po::variables_map& vm
) {
  // Build and bind a tokenizer to the input file
  arrow::Tokenizer tokenizer{is, vm["input-file"].as<std::string>()};

  // Enumerate until the end-of-file ..
  for (;;) {
    // .. read each token ..
    auto tok = tokenizer.pop();

    // .. and print them to stdout
    std::printf("%s: %s\n", tok->span.to_string().c_str(), tok->to_string().c_str());

    if (tok->type == arrow::Token::Type::End) { break; }
  }

  return arrow::Log::get().count("error") > 0 ? EXIT_FAILURE : 0;
}

}  // namespace command
}  // namespace arrow
