// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/parser.hpp"

namespace arrow {

// Pattern
// TODO: `pattern-unit` and `pattern-clause` are terrible names
// -----------------------------------------------------------------------------
// pattern-literal = FLOAT | BOOLEAN | INTEGER | STRING ;
// pattern-binding = [ "mutable" ] IDENTIFIER ;
// pattern-unit = pattern-binding | pattern-literal | "_" ;
// pattern-clause = pattern-unit
//    | "(" ")"
//    | "(" pattern-unit "," ")"
//    | "(" pattern-unit "," pattern-unit "," pattern-unit [ "," ] ")"
//    ;
// pattern = pattern-clause { "," pattern-clause } ;
// -----------------------------------------------------------------------------
bool Parser::parse_pattern() {
  let _ = 30;

  return false;
}

}  // namespace arrow
