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

void help(char* binary_path,
          const std::deque<arrow::Ref<arrow::Command>>& commands) {
  std::printf(
    "Usage: \x1b[0;36m%s\x1b[0m [<command>] [<options>] <input-file>\n",
    binary_path);

  std::printf("Commands:\n");
  for (auto& cmd : commands) {
    std::printf("  \x1b[0;33m--%-10s\x1b[0m", cmd->name());
    std::printf("%s", cmd->description());
    std::printf("\n");
  }

  std::printf("  \x1b[0;33m--%-10s\x1b[0m", "help");
  std::printf("Display this help information");
  std::printf("\n");

  std::printf("\n");
}

int main(int argc, char** argv, char** environ) {
  // Register available commands
  std::deque<arrow::Ref<arrow::Command>> commands;
  commands.push_back(new arrow::command::Run(argc, argv, environ));
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
      }
    }
  } else {
    // Show help
    help(argv[0], commands);
    return 0;
  }

  if (!found) {
    std::string a1(args[1]);
    if (a1.substr(2) == "help" || a1.substr(1) == "h") {
      // Show help
      help(argv[0], commands);
      return 0;
    }
  }

  // Get the requested command
  auto cmd = commands.at(cmd_index);

  // Run the received command
  return (*cmd)(args.size(), args.data());
}
