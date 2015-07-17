// [x] type->name()
// [x] type->equals()

// tuple unpack checks

// [x] fail #1 - match initializer against annotation (if present)
// NOTE: This does NOT need recursive AST context for error reporting; it would
//       be a simple type equality check
//  => "expected `((int,), bool)`, found `(float,)`"

// [x] fail #2 - match pattern against initializer
// NOTE: This does need recursive AST context for error reporting
//  => "expected a tuple with 2 elements, found one with 3 elements"

// [x] unpack constant tuple
// [x] unpack slot (with tuple type)
// [x] recursive unpack constant tuple
// [x] recursive unpack slot (with tuple type)

// [x] optimize literal tuple pack/unpack with `code::ValueTuple`
// [x] pack non-constant values

// [x] assignment (refactor most of the slot code into a "do_assign" method)

// [x] `None` type and value

// [.] Type coercion and promotion (int32 -> int64 -> float / etc.) [*]
//    [x] Generate integer literals to the smallest type
//    [x] Implement is_coercible_to
//    [ ] Enforce that slots have `int` unless with explicit annotation
//    [ ] Implement common_type (better name?)
//    [ ] Use common_type (?) to reduce assignment types in analyze
//    [-] Implement cast (implicit and explicit)
//    [ ] Add (tuple,) to (tuple,) logic to is_coercible_to and do_cast
//    [ ] Allow [ast::Slot] to be type-resolved

// ------------ function
// [ ] function declaration
// [ ] function type resolution
// [ ] parameters
// [ ] mutable parameters
// [ ] result type / return statement

// ------------ extern [**]
// [x] extern slot
// [ ] extern function

// ------------ call [***]
// [ ] call
// [ ] call: keyword arguments
// [ ] call: default arguments

// ------------ import
// [ ] import
// [ ] mutually recursive import (for functions)
// [ ] mutually recursive import (for slots)

// ------------ contextual inference
// [x] initial
// [x] error for unresolved
// [ ] error for use without assign
// [ ] warn for no use for assign
// [ ] warn for no assign for declare
// [ ] block scoping [****]
// [ ] closures

// ------------ atom-arrow
// [x] float
// [x] variables shouldn't require space before "let"
// [x] export/extern before "let" and "def"
// [x] 0-prefixed numbers don't highlight
// [x] patterns
// [ ] function parameters
// [ ] Capture keywords outside of meta patterns
// [ ] Annotations
