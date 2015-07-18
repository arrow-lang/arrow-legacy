// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_COMMAND_RUN_H
#define ARROW_COMMAND_RUN_H 1

#include "arrow/command.hpp"

namespace arrow {
namespace command {

class Run : public Command {
 public:
  Run(int argc, char** argv, char** environ)
    : _argc(argc), _argv(argv), _environ(environ) {
  }

  virtual ~Run() noexcept { }

  const char* name() const noexcept { return "run"; }
  const char* description() const noexcept {
    return "Compile (and execute) the input file";
  }

 private:
  virtual int run(
    std::shared_ptr<std::istream> is,
    const boost::program_options::variables_map& vm);

  int _argc;
  char** _argv;
  char** _environ;
};

}  // namespace command
}  // namespace arrow

#endif  // ARROW_COMMAND_RUN_H
