// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/command.hpp"

namespace po = boost::program_options;

namespace arrow {

Command::~Command() noexcept { }

int Command::operator()(int argc, char** argv) {
  // Declare "default" options
  po::options_description desc;
  desc.add_options()("input-file", po::value<std::string>());

  // Allow commands to add their own options
  add_options(desc);

  // Declare the positional argument (the input file)
  po::positional_options_description p;
  p.add("input-file", 1);


  try {
    // Process command-line arguments against the described options
    po::variables_map vm;
    po::store(
      po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    po::notify(vm);

    // Run our command
    return run(vm);
  } catch(po::unknown_option&) {
    // TODO: Report error;
    return EXIT_FAILURE;
  }
}

}  // namespace arrow
