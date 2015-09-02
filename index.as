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

// [ ] Type alias
// [ ] Fold (run after analyze-usage but before analyze-module)

// [-] Call {*}
//    [x] Mutable parameters
//    [x] Check for mutable parameters (to ensure captured arg is mutable)
//    [x] Check for invalid keyword parameters
//    [ ] Check for missing parameters {*}
//    [ ] Check for too many parameters {*}
//    [ ] Aggregate by-const-reference (struct, array, tuple, etc.) {*}
//    [ ] Default arguments {*}
//    [ ] Keyword arguments {*}

// [ ] Varidac for extern "C" functions {*}

// ------------

// // NOTE: This is how the std will request an intrinsic (read: builtin) function
// //       from the compiler.
// extern "intrinsic" def __cos_f32(value: float32) -> float32;
//
// trait Trignometry {
//   def cos(self) -> Self;
//   def sin(self) -> Self;
//   def tan(self) -> Self;
// }
//
// implement Trignometry for float32 {
//   def cos(self) -> Self {
//     return __cos_f32(self);
//   }
// }
//
// // NOTE: `-> type(..)` isn't needed as return types are auto-resolved but
// //       just showing for completeness
// def cos(value: Trignometry) -> type(value.cos()) {
//   return value.cos();
// }

// ------------
