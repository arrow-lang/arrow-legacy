// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_COMMAND_COMPILE_H
#define ARROW_COMMAND_COMPILE_H 1

#include "arrow/command.hpp"

namespace arrow {
namespace command {

class Compile : public Command {
 public:
  virtual ~Compile() noexcept { }

  const char* name() const noexcept { return "compile"; }
  const char* description() const noexcept {
    return "Compile and assemble the input file";
  }

 private:
  virtual void add_options(boost::program_options::options_description&);

  virtual int run(
    std::shared_ptr<std::istream> is,
    const boost::program_options::variables_map& vm);
};

}  // namespace command
}  // namespace arrow

#endif  // ARROW_COMMAND_COMPILE_H
