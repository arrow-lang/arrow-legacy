// [ ] type->name()

// tuple unpack checks

// [ ] fail #1 - match initializer against annotation (if present)
// NOTE: This does NOT need recursive AST context for error reporting; it would
//       be a simple type equality check
//  => "expected `((int,), bool)`, found `(float,)`"

// [ ] fail #2 - match pattern against initializer
// NOTE: This does need recursive AST context for error reporting
//  => "expected a tuple with 2 elements, found one with 3 elements"

// [ ] unpack constant tuple
// [ ] unpack slot (with tuple type)
// [ ] recursive unpack constant tuple
// [ ] recursive unpack slot (with tuple type)
// [ ] pack non-constant values

// [ ] assignment (refactor most of the slot code into a "do_assign" method)

// [ ] `None` type and value

// [ ] function declaration
// [ ] function type resolution
// [ ] parameters
// [ ] mutable parameters
// [ ] result type / return statement

// [ ] extern slot
// [ ] extern function

// [ ] call
// [ ] call: keyword arguments
// [ ] call: default arguments

// [ ] import
// [ ] mutually recursive import (for functions)
// [ ] mutually recursive import (for slots)
