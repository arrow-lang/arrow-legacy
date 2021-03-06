// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <vector>
#include <string>
#include <boost/filesystem.hpp>

#include "arrow/command/compile.hpp"
#include "arrow/compiler.hpp"
#include "arrow/log.hpp"

namespace fs = boost::filesystem;
namespace po = boost::program_options;

namespace arrow {
namespace command {

void Compile::add_options(boost::program_options::options_description& desc) {
  desc.add_options()
    ("no-verify", "");
}

int Compile::run(
  std::shared_ptr<std::istream> is,
  const po::variables_map& vm
) {
  // Check if verification has been requested to be disabled
  auto no_verify = vm.count("no-verify") > 0;

  // Build and bind a parser to the input file
  auto filename = vm["input-file"].as<std::string>();
  arrow::Parser parser{is, filename};

  // Parse the file into a single, top-level node (module)
  auto node = parser.parse();
  if (arrow::Log::get().count("error") > 0) {
    return EXIT_FAILURE;
  }

  // Build a compiler
  arrow::Compiler compiler(!no_verify);
  compiler.initialize();

  // Compile the top-level node (module)
  auto module_name = fs::path(filename).stem().string();
  compiler.compile(module_name, node);
  if (arrow::Log::get().count("error") > 0) {
    return EXIT_FAILURE;
  }

  // Show the generated assembly
  compiler.print();

  return 0;
}

}  // namespace command
}  // namespace arrow
