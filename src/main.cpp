// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <cstdlib>
#include <deque>
#include <vector>
#include <string>

#include "arrow/command.hpp"
#include "arrow/ref.hpp"
#include "arrow/log.hpp"

int main(int argc, char** argv) {
  // Register available commands
  std::deque<arrow::Ref<arrow::Command>> commands;
  commands.push_back(new arrow::command::Compile());
  commands.push_back(new arrow::command::Parse());
  commands.push_back(new arrow::command::Tokenize());
  commands.push_back(new arrow::command::Read());

  // Check for a specified command
  std::vector<char*> args(argv, argv + argc);
  unsigned cmd_index = 0;
  bool found = false;
  if (args.size() >= 2) {
    std::string a1(args[1]);
    if (a1.size() >= 3 && a1[0] == '-' && a1[1] == '-') {
      // The first arguments is `--[...]`
      auto command_name = a1.substr(2);
      for (unsigned i = 0; i < commands.size(); ++i) {
        if (command_name == commands[i]->name()) {
          found = true;
          cmd_index = i;
          break;
        }
      }

      if (found) {
        // Remove the command argument
        args.erase(args.begin() + 1);
      } else {
        arrow::Log::get().error("unrecognised option '%s'", a1.c_str());
        return EXIT_FAILURE;
      }
    }
  }

  // Get the requested command
  auto cmd = commands.at(cmd_index);

  // Run the received command
  return (*cmd)(args.size(), args.data());
}
