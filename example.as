// Track
// -----
// [ ] _current_module
// [ ] _current_loop (stack)
// [ ] _current_function (stack)

// Operator
// --------
// [ ] `.`
// [ ] `and`
// [ ] `or`
// [ ] `not`

// [ ] Import (from source)

// Anonymous Functions
// -------------------
// [ ] Type
// [ ] Allocation
// [ ] Assignment (to existing function)
// [ ] Application
// [ ] Expression

// [ ] Block
// [ ] Break / Continue
// [ ] Mutable parameter

import ctypes from "./lib/libc/ctypes";
// import other from "./other";

extern def printf(format: str, a: int);
extern def puts(s: str);

struct Point {
  x: int,
  y: int128
}

def main() {
  let size = (((0 as *Point) + 1) as *byte) - (0 as *byte);
  printf("size: %lld", size);
  puts("");
}
