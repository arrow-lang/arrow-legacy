// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <iostream>
#include <string>
#include "arrow/tokenizer.hpp"
#include "arrow/parser.hpp"
#include "arrow/generator.hpp"
#include "arrow/log.hpp"
#include "boost/program_options.hpp"
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;
namespace po = boost::program_options;
namespace ast = arrow::ast;
using arrow::Log;
using arrow::Parser;
using arrow::Tokenizer;
using arrow::Generator;
using arrow::Token;
using std::printf;

void print_help(const char* binary_path) {
  printf("Usage: ");
  printf("\x1b[36m%s\x1b[0m", binary_path);
  printf(" [options] <file>\n");

  printf("Options:\n");

  static const auto fmt = "    \x1b[33m%-20s\x1b[0m %s\n";
  printf(fmt, "-h, --help", "Display help information");
  printf(fmt, "-V, --version", "Output version");
  printf(fmt, "--tokenize", "Tokenize the input file and print the tokens");
  printf(fmt, "--parse", "Parse the input file and print the AST");

  printf("\n");
}

int main(int argc, char** argv) {
  // arrow < {input} > {output}

  // Declare supported options
  po::options_description desc;
  desc.add_options()
      ("help,h", "")
      ("version,V", "")
      ("tokenize", "")
      ("parse", "")
      ("input-file", po::value<std::string>(), "");

  // Declare some options as positional
  po::positional_options_description p;
  p.add("input-file", 1);

  // Process command-line arguments against the described options
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).
            options(desc).positional(p).run(), vm);
  po::notify(vm);

  if (vm.count("help") || argc <= 1) {
    print_help(argv[0]);
    return 0;
  }

  if (vm.count("version")) {
    // TODO(mehcode): Should reference a library value for the version
    // TODO(mehcode): Should include a git sha / tag (git describe)
    // TODO(mehcode): Should include build platform
    // TODO(mehcode): Should include build date
    printf("Arrow 0.1.0-pre\n");
    return 0;
  }

  if (!vm.count("input-file")) {
    Log::get().error("no input filename given");
    return EXIT_FAILURE;
  }

  // Construct the tokenizer
  auto tokenizer = Tokenizer(vm["input-file"].as<std::string>());
  if (Log::get().count("error") > 0) { return EXIT_FAILURE; }

  if (vm.count("tokenize")) {
    for (;;) {
      auto tok = tokenizer.pop();

      printf("%s: ", tok->span.to_string().c_str());

      if (tok->type == Token::Type::Integer) {
        auto tok_ = std::static_pointer_cast<arrow::IntegerToken>(tok);
        printf("integer: %s (%d)\n", tok_->text.c_str(), tok_->base);
      } else if (tok->type == Token::Type::Float) {
        auto tok_ = std::static_pointer_cast<arrow::FloatToken>(tok);
        printf("float: %s\n", tok_->text.c_str());
      } else if (tok->type == Token::Type::Identifier) {
        auto tok_ = std::static_pointer_cast<arrow::IdentifierToken>(tok);
        printf("identifier: %s\n", tok_->text.c_str());
      } else {
        printf("%s\n", arrow::to_string(tok->type).c_str());
      }

      if (tok->type == arrow::Token::Type::End) { break; }
    }

    return EXIT_SUCCESS;
  }

  // Construct the parser
  Parser parser{tokenizer};

  // Parse into a module node
  auto module = parser.parse();
  if (Log::get().count("error") > 0) { return EXIT_FAILURE; }

  if (vm.count("parse")) {
    // Show the AST
    ast::Show(std::cout).run(*module);

    return EXIT_SUCCESS;
  }

  // Construct the generator
  Generator generator{};

  // Generate the IR
  auto name = fs::path(tokenizer.filename()).stem().string();
  generator.generate(name, module);
  if (Log::get().count("error") > 0) { return EXIT_FAILURE; }

  // Build the main function
  // NOTE: This wouldn't be done if this was to be a library
  generator.generate_main();
  if (Log::get().count("error") > 0) { return EXIT_FAILURE; }

  // Print the IR
  generator.print(std::cout);

  return EXIT_SUCCESS;
}
