// [ ] When the RHS of an assignment or an argument expression to
//     call evaluates to a TypeFunction; apply the use checks

// Issues:
// [ ] All integral literals should resolve to <int>
// [ ] Integral literals should cast to <any int> as long as the type is
//     wide enough for the literal (keep in mind negative literals)

// Loop:
// [ ] Loop
// [ ] While / Until
// [ ] Break
// [ ] Continue

// Structure:
// [ ] Expose (name)
// [ ] AnalyzeType: TypeStructure
// [ ] Resolve (type_of(Member))
// [ ] AnalyzeUsage (check member access)
// [ ] Declare
// [ ] Build (path: struct.member)
// [ ] Build (call: struct())

// Improvements (General):
// [ ] Bitwise (binary) operations
// [ ] Explicit Cast: as
// [ ] Logical Combinator: and or
// [ ] Conditional (ternary) expression
// [ ] CTA for tuples

// language-arrow:
// [ ] Function parameters
// [ ] Assignment

let a = 10 as float;
