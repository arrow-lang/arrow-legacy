// [ ] Type alias
// [ ] Fold (run after analyze-usage but before analyze-module)

// [-] Call
//    [ ] Aggregate by-const-reference (struct, array, tuple, etc.)
//    [ ] Default arguments

// [ ] Varidac for extern "C" functions

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
