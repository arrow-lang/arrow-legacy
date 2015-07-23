// AnalyzeUsage
//  [ ] Function
//    [x] Basic: nested analyze
//    [ ] Non-local assignment/use
//  [ ] Loop
//  [ ] Import
//  [ ] Pointers

// Build
//  [x] Function
//  [ ] Return (Explicit)
//  [ ] Return (Implicit)

// Case 1
// ======

let a;

def method() {
  a = 10;
}

method(); // ok
method(); // error

// Case 2
// ======

let a;
