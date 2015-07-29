// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_COMMAND_READ_H
#define ARROW_COMMAND_READ_H 1

#include <string>

#include "arrow/command.hpp"

namespace arrow {
namespace command {

class Read : public Command {
 public:
  virtual ~Read() noexcept { }

  const char* name() const noexcept { return "read"; }
  const char* description() const noexcept {
    return "Read the input file and print each UTF-32 character";
  }

 private:
  virtual int run(
    std::shared_ptr<std::istream> is,
    const boost::program_options::variables_map& vm);
};

}  // namespace command
}  // namespace arrow

#endif  // ARROW_COMMAND_READ_H
