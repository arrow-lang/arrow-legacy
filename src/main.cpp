// #include <cstdio>
#include "arrow/tokenizer.hpp"
#include "boost/program_options.hpp"

namespace po = boost::program_options;
using namespace arrow;
using std::printf;

void print_help(const char* binary_path) {
  printf("Usage: ");
  printf("\x1b[36m%s\x1b[0m", binary_path);
  printf(" [<options>] <file>\n");

  printf("Options:\n");

  static const auto fmt = "    \x1b[33m%-20s\x1b[0m %s\n";
  printf(fmt, "-h, --help", "Display help information");
  printf(fmt, "-V, --version", "Output version");

  printf("\n");
}

int main(int argc, char** argv) {
  // arrow < {input} > {output}

  // Declare supported options
  po::options_description desc;
  desc.add_options()
      ("help,h", "")
      ("version,V", "")
      ("input-file", po::value<std::string>(), "")
  ;

  // Declare some options as positional
  po::positional_options_description p;
  p.add("input-file", 1);

  // Process command-line arguments against the described options
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).
            options(desc).positional(p).run(), vm);
  po::notify(vm);

  if (vm.count("help")) {
    print_help(argv[0]);
    return 0;
  }

  // Tokenize!
  // FIXME: This should be inside a parser inside a compiler .. etc.
  //    We'll get to that

  auto tokenizer = Tokenizer(vm["input-file"].as<std::string>());

  for (;;) {
    auto tok = tokenizer.next();

    printf("%s: ", tok->span.to_string().c_str());

    if (tok->type == Type::Integer) {
      auto tok_ = std::static_pointer_cast<IntegerToken>(tok);
      printf("integer: %s (%d)\n", tok_->text.c_str(), tok_->base);
    } else if (tok->type == Type::Float) {
      auto tok_ = std::static_pointer_cast<FloatToken>(tok);
      printf("float: %s\n", tok_->text.c_str());
    } else if (tok->type == Type::Identifier) {
      auto tok_ = std::static_pointer_cast<IdentifierToken>(tok);
      printf("identifier: %s\n", tok_->text.c_str());
    } else {
      printf("%s\n", arrow::to_string(tok->type).c_str());
    }

    if (tok->type == arrow::Type::End) { break; }
  }
}
