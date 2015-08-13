
// Loop:
// [ ] Loop
// [ ] While / Until
// [ ] Break
// [ ] Continue

// Structure:
// [x] Expose (name)
// [x] AnalyzeType: TypeStructure
// [x] Resolve (type_of(Member))
// [x] AnalyzeUsage (check member access)
// [x] Declare
// [ ] AnalyzeUsage (check mutability)
// [ ] Build (path: struct.member)
// [ ] Build (call: struct())

// Improvements (General):
// [ ] Bitwise (binary) operations
// [x] Explicit Cast: as
// [ ] Logical Combinator: and or
// [ ] Conditional (ternary) expression
// [ ] CTA for tuples

// language-arrow:
// [ ] Function parameters
// [ ] Assignment

struct A { b: *B }
struct B { a: *A }

def main() {
  let a: A;
  let b: B;
}
