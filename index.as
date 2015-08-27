// [.] Array
//    [x] Parse
//    [x] Type
//    [x] Build/Resolve literal
//    [x] Index (Get)
//    [x] Index (Set)
//    [ ] Mutability checking for index (set)
//    [ ] Omit size `[_]`

// [x] Explicit Cast: as
// [ ] Type alias
// [ ] Fold (run after analyze-usage but before analyze-module)

// [-] Call
//    [x] Mutable parameters
//    [ ] Check for mutable parameters (to ensure captured arg is mutable) {*}
//    [ ] C ABI parameters
//    [ ] Aggregate by-const-reference (struct, array, tuple, etc.) {*}
//    [ ] Default arguments {*}
//    [ ] Keyword arguments {*}

// [ ] Instance/Attached methods (implement)
// [ ] Destructors (RIIA)
// [ ] Generics
//    [ ] Generic structs
//    [ ] Generic implement (which creates generic functions)

// [ ] Command line arguments to script

// TODO(arrow): Varidac for extern "C" functions {*}

// TODO(arrow):
//    Absolute module imports that resolve from an installation
//    folder (eg. so we can do `import libc from "libc"`)

// [ ] Export modules (through `use X = mod` or `export X from ".."`)
