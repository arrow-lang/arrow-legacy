// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_COMMAND_READ_H
#define ARROW_COMMAND_READ_H 1

#include <memory>
#include <istream>
#include <string>
#include <deque>

#include "arrow/command.hpp"

namespace arrow {
namespace command {

class Read : public Command {
 public:
  virtual ~Read() noexcept { }

  const char* name() const noexcept { return "read"; }
  const char* description() const noexcept {
    return "Read the input file and print each interpreted UTF-32 character";
  }

 private:
  virtual int run(boost::program_options::variables_map&);
  virtual void add_options(boost::program_options::options_description&);

};

}  // namespace command
}  // namespace arrow

#endif  // ARROW_COMMAND_READ_H
