
// Issues:
// [ ] All integral literals should resolve to <int>
// [ ] Integral literals should cast to <any int> as long as the type is
//     wide enough for the literal (keep in mind negative literals)

// Function:
// [ ] Move analyze to the item (not the type)
// [ ] Run usage/assign analysis on function assignment (to prevent "leakage")
// [ ] Parse/Type TypeFunction annotations

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
// [ ] Logical operaetors (and / or)
// [ ] Conditional (ternary) expression
// [ ] CTA for tuples

// language-arrow:
// [ ] Function parameters
// [ ] Assignment


extern def puts(s: str);

let mutable name: str;

def method() { name = "Ryan"; }

method();

puts(name);
