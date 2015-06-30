// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <string>
#include <fstream>

#include "arrow/command.hpp"
#include "arrow/log.hpp"

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
    po::store(po::command_line_parser(argc, argv)
      .options(desc)
      .positional(p)
      .run(),
    vm);
    po::notify(vm);

    // Bind our input_file to an input_stream
    // TODO(mehcode): Check for no file passed (and use stdin)
    auto input_file = vm["input-file"].as<std::string>();
    auto input_fs = new std::ifstream(input_file);
    if (!input_fs->is_open()) {
      Log::get().error(
        "couldn't read \"%s\"; couldn't open path as file", input_file.c_str());

      return EXIT_FAILURE;
    }

    std::shared_ptr<std::istream> input_stream(input_fs);

    // Run our command
    return run(input_stream, vm);
  } catch(po::unknown_option& uo) {
    Log::get().error(uo.what());
    return EXIT_FAILURE;
  }
}

}  // namespace arrow
