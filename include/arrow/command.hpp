// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_COMMAND_H
#define ARROW_COMMAND_H 1

#include <memory>
#include <istream>
#include <string>
#include <deque>

#include "boost/program_options.hpp"

namespace arrow {

/// Represents an executable command to be ran from the command-line driver.
class Command {
 public:
  static std::deque<std::shared_ptr<Command>> commands;

  virtual ~Command() noexcept;

  virtual const char* name() const noexcept = 0;
  virtual const char* description() const noexcept = 0;

  /// Run the command and return its result.
  int operator()(int argc, char** argv);

 private:
  virtual int run(boost::program_options::variables_map&) = 0;
  virtual void add_options(boost::program_options::options_description&) = 0;

};

}  // namespace arrow

#include "arrow/command/read.hpp"

#endif  // ARROW_COMMAND_H
