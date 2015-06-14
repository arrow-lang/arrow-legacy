// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/command.hpp"

int main(int argc, char** argv) {
  // Register available commands
  std::deque<std::shared_ptr<arrow::Command>> commands;
  commands.push_back(std::make_shared<arrow::command::Read>());

  // Get the requested command
  auto cmd = commands.at(0);

  // Run the received command
  return (*cmd)(argc, argv);
}
