// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_COMMAND_PARSE_H
#define ARROW_COMMAND_PARSE_H 1

#include <string>

#include "arrow/command.hpp"

namespace arrow {
namespace command {

class Parse : public Command {
 public:
  virtual ~Parse() noexcept { }

  const char* name() const noexcept { return "parse"; }
  const char* description() const noexcept {
    return "Parse the input file and print the AST (encoded in JSON)";
  }

 private:
  virtual int run(
    std::shared_ptr<std::istream> is,
    const boost::program_options::variables_map& vm);
};

}  // namespace command
}  // namespace arrow

#endif  // ARROW_COMMAND_PARSE_H
