def main() {
  // printf("Hello World");
}

// [ ] Type annotations
// [ ] Parameters
// [ ] Return

// [ ] External function declarations
extern "C" def printf(str, ...) -> int32;

// [ ] Slots (immutable)
let var: int32 = 30;

// [ ] Type inference (for slot definitions)
let var = 30;

// [ ] Selection statements
if some_condition {
} else if other_condition {
} else {
}

// [ ] Contextual type inference (for slot definitions)
let var;
if some_condition {
  var = false;
} else {
  var = true;
}

// [ ] Binary and unary expressions (math)

// [ ] Modules (Source)
import std;
import bardo from "@mehcode/bardo";
import path from "../relative/path";
