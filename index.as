// Sept. 4:
//    - Finish "Call", "Type Alias" (basic), and "Fold"
//    - Test build/install on:
//        > Linux (amd64)
//        > Linux (x86)
//        > Linux (ARM)
//        > Windows 7 (x86)
//        > Windows 7 (amd64)
//        > Windows 8 (amd64)
//        > Windows 10 (amd64)
//        > Mac OSX [whatever is latest]
//    - Finish varidac for extern-c
//    - Instance/Attached methods
//    - Basic IO (simple libc wrapper)
//    - prelude (and assert)

// ------------

// [x] Explicit Cast: as
// [ ] Type alias
// [ ] Fold (run after analyze-usage but before analyze-module)

// [-] Call
//    [x] Mutable parameters
//    [ ] Check for mutable parameters (to ensure captured arg is mutable) {*}
//    [ ] Aggregate by-const-reference (struct, array, tuple, etc.) {*}
//    [ ] Default arguments {*}
//    [ ] Keyword arguments {*}

// [ ] Instance/Attached methods (implement)
// [ ] Destructors (RIIA)
// [ ] Traits
// [ ] Generics
//    [ ] Generic structs
//    [ ] Generic implement (which creates generic functions)

// [ ] Varidac for extern "C" functions {*}

// [ ] Intrinsics (requires deciding on overloaded functions)
//    [ ] pow
//    [ ] cos/sin/tan
//    [ ] shift/bswap

// Discuss:
// [ ] Overloaded functions (yes/no)
//      - pow(float, int) vs pow(float, float)
// [ ] Member functions or "free" functions
//      - math.pow(3.12, 1) vs (3.12).pow(1)
//      - math.sqrt(3.5) vs (3.5).sqrt()
//      - math.is_finite(4.12) vs (4.12).is_finite()

// Libraries:
// [ ] math
// [ ] random
// [ ] string
// [ ] bits
// [ ] time / calendar / date / datetime / chrono
// [ ] platform
// [ ] signal
// [ ] io
