// [x] Bitwise (binary) operations
// [x] do_logical comparisons for bool
// [x] do_logical Combinator: and or
// [x] Conditional (ternary) expression

// [.] Array
//    [x] Parse
//    [x] Type
//    [x] Build/Resolve literal
//    [ ] Index (Get)
//    [ ] Index (Set)
//    [ ] Omit size `[_]`

// [x] Pointer
//    [x] Dereference (Set)
//    [x] Mutability checking
//    [x] Arithmetic
//    [x] Relational (==, !=, >=, >, <=, <)

// [x] Explicit Cast: as
// [ ] Type alias
// [ ] Fold (run after analyze-usage but before analyze-module)

// [-] Call
//    [x] Mutable parameters
//    [ ] Check for mutable parameters (to ensure captured arg is mutable)
//    [ ] C ABI parameters
//    [ ] Aggregate by-const-reference (struct, array, tuple, etc.)
//    [ ] Default arguments
//    [ ] Keyword arguments

// [ ] Instance/Attached methods (implement)
// [ ] Destructors (RIIA)
// [ ] Generics
//    [ ] Generic structs
//    [ ] Generic implement (which creates generic functions)

// [ ] Command line arguments to script

// TODO(arrow): Varidac for extern "C" functions

// TODO(arrow):
//    Absolute module imports that resolve from an installation
//    folder (eg. so we can do `import libc from "libc"`)

// [ ] Export modules (through `use X = mod` or `export X from ".."`)
