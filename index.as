
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
// [ ] Logical operaetors (and / or)
// [ ] Conditional (ternary) expression
// [ ] CTA for tuples

// language-arrow:
// [ ] Function parameters
// [ ] Assignment

extern def puts(s: str);

let mutable name: str = "";

def method(a: int) -> str { return "Ryan"; }
def call(fn: (int) -> str, a: int) { name = fn(a); }

call(method, 30);

puts(name);
