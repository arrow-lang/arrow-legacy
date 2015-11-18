import stdio from "../lib/libc/stdio";
import string from "./util/string";

def main(argc: int, argv: *str) -> int {
  // Parse arguments and determine the command
  // Expect the command as the first argument.. eg. `arrow --compile` or
  //    `arrow -V`
  let mutable idx = 1;
  let mutable command = command_help;

  if (argc > 1) {
    let cmd = *(argv + idx);

    if string.eq(cmd, "--help") or string.eq(cmd, "-h") or string.eq(cmd, "-?") {
      command = command_help;
    } else if string.eq(cmd, "--read") {
      command = command_read;
    } else {
      // TODO: log.error("")
      stdio.printf("arrow: \x1b[0;31merror:\x1b[0m \x1b[1;37munrecognised command '%s'\x1b[0m\n", cmd);
      return 1;
    }

    idx += 1;
  }

  return command();
}

def command_help() -> int {
  // Usage
  stdio.puts(
    "Usage: \x1b[0;36marrow\x1b[0m [<command>] [<options>] <input-file>");

  // Commands
  stdio.puts("Commands:");
  stdio.puts("  \x1b[0;33m--read\x1b[0m      Read the input file and print each UTF-32 character");
  stdio.puts("  \x1b[0;33m--help, -h\x1b[0m  Display this help information");

  stdio.puts("");

  return 0;
}

def command_read() -> int {
  return 0;
}
