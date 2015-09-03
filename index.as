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

// [ ] Varidac for extern "C" functions

// [ ] Error: And/Or inside of if and while
// [ ] Check break/continue when not in a loop

// [ ] Intrinsic
// NOTE: This is how the std will request an intrinsic (read: builtin) function
//       from the compiler.
// extern "intrinsic" def __cos_f32(value: float32) -> float32;
//    [ ] Parse (add ABI string)
//    [ ] Build/Expose/etc.
//        [ ] shl
//        [ ] lshr
//        [ ] ashr
//        [ ] sqrt
//        [ ] pow
//        [ ] powi
//        [ ] sin
//        [ ] cos
//        [ ] exp
//        [ ] exp2
//        [ ] log
//        [ ] log10
//        [ ] log2
//        [ ] fabs
//        [ ] fmin
//        [ ] fmax
//        [ ] floor
//        [ ] ceil
//        [ ] round
//        [ ] bswap
//        [ ] ctpop
//        [ ] ctlz
//        [ ] cttz

// [ ] Traits
//    [ ] Parse

// [ ] Implement (Attached functions)
//    [ ] Parse
//    [ ] Build/Expose/etc.

// [ ] Implement (Instance methods)
//    [ ] Parse
//    [ ] Build/Expose/etc.

// [ ] Implement (Trait methods)
//    [ ] Parse
//    [ ] Build/Expose/etc.


// ------------

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
