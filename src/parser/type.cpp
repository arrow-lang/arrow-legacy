// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include "arrow/parser.hpp"

namespace arrow {

// Type
// -----------------------------------------------------------------------------
// type = identifier
//      | "None"
//      | "*" type
//      | "(" ")"
//      | "(" type { "," type } [ "," ] ")"
//      | function-type
//      ;
// function-type-parameter = [ "identifier" ":" ] type ;
// function-type = "(" ")" "->" type
//               | "(" function-type-parameter { "," function-type-parameter } [ "," ] ")" "->" type
//               ;
// -----------------------------------------------------------------------------
bool Parser::parse_type() {
  return false;
}

}  // namespace arrow
