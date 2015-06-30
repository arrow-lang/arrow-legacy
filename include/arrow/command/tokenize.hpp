// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_COMMAND_TOKENIZE_H
#define ARROW_COMMAND_TOKENIZE_H 1

#include "arrow/command.hpp"

namespace arrow {
namespace command {

class Tokenize : public Command {
 public:
  virtual ~Tokenize() noexcept { }

  const char* name() const noexcept { return "tokenize"; }
  const char* description() const noexcept {
    return "Tokenize the input file and print each token";
  }

 private:
  virtual int run(
    std::shared_ptr<std::istream> is,
    const boost::program_options::variables_map& vm);
};

}  // namespace command
}  // namespace arrow

#endif  // ARROW_COMMAND_TOKENIZE_H
