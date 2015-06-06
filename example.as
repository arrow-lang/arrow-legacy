// [x] Fix `no implicit conversion from '*mutable uint8' to '*mutable uint8'`
// [x] Pointer arithmetic (+, -)
// [x] Pointer difference (size of)
// [ ] Pointer comparison
// [ ] Pointer (explicit) cast
// [ ] `not` parse
// [ ] Selection "expression"
// [ ] Ternary expression
// [ ] Short-circuiting logical combinators
// [x] De-duplicate AST nodes and parse compound assignment as two nodes
// [ ] Structures

extern def puts(s: str);
extern def malloc(size: uint32) -> *mutable byte;

def main() -> uint64 {
  let m = malloc(40);
  let n = m + 1;
  m == n;
  return 0;
}
