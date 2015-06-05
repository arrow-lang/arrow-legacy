// [ ] Fix `no implicit conversion from '*mutable uint8' to '*mutable uint8'`
// [ ] Pointer arithmetic (+, -)
// [ ] Pointer comparison
// [ ] Pointer (explicit) cast
// [ ] `not` parse
// [ ] Selection "expression"
// [ ] Ternary expression
// [ ] Short-circuiting logical combinators
// [ ] De-duplicate AST nodes and parse compound assignment as two nodes
// [ ] Structures

extern def puts(s: str);
extern def malloc(size: uint32) -> *mutable byte;

def main() -> int {
  let mutable m: *mutable byte = malloc(2);
  // *(m + 0) = 10;
  // *(m + 1) = 20;
}
