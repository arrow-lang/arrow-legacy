// [ ] Operator precedence (incl. lots of parse tests)
// [ ] Parsing of logical `not`
// [ ] Structures
// [ ] Explicit type deduction (`type(x)`)
// [ ] Modules (source)
// [ ] First-class functions
// [x] Blocks
//  > [ ] Switch to using shared_ptr<Scope> for ALL scope references
//  > [ ] Make an alias as arrow::ptr<T> and arrow::ptr_cast<T> (dynamic_pointer_cast)


// [ ] Module initilization
// [ ] Global variables
// [ ] Break / Continue
// [ ] Alias `use x = y` `use [x = ] mod.x`
// [ ] Short-circuiting logical combinators
// [ ] Default/Keyword arguments


extern def puts(s: str);

// Make this be a thing
// Module init happens BEFORE `main`
// Module init (among various modules) has an undefined order except in
// relation to exported slot initilization which will attempt to re-order
// module initilization functions on dependencies
// It does not re-order lines in the module init functions; just the
// functions themselves

let mutable msg = "Hello World";
puts(msg);

def main() -> uint32 {
  {
    {
      let mutable y = 10;
      let x = { y += 20; 40; };
      return y;
    }
  }
}
