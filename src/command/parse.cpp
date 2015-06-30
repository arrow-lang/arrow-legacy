// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <vector>
#include <string>

#include "arrow/command/parse.hpp"
#include "arrow/parser.hpp"
#include "arrow/ast.hpp"
#include "arrow/ast/show.hpp"
#include "arrow/log.hpp"

namespace po = boost::program_options;

namespace arrow {
namespace command {

int Parse::run(
  std::shared_ptr<std::istream> is,
  const po::variables_map& vm
) {
  // Build and bind a parser to the input file
  arrow::Parser parser{is, vm["input-file"].as<std::string>()};

  // Parse the file into a single, top-level node (module)
  auto node = parser.parse();
  if (arrow::Log::get().count("error") > 0) {
    return EXIT_FAILURE;
  }

  // Show the AST
  ast::Show().run(*node);

  return 0;
}

}  // namespace command
}  // namespace arrow
