// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_SPAN_H
#define ARROW_SPAN_H 1

#include <string>

namespace arrow {

struct Position {
  Position(unsigned row, unsigned column);

  Position operator +(unsigned offset);
  Position operator -(unsigned offset);

  unsigned row;
  unsigned column;
};

struct Span {
  Span(const std::string& filename, Position begin, Position end);

  std::string to_string() const;

  std::string filename;
  Position begin;
  Position end;
};

}  // namespace arrow

#endif  // ARROW_SPAN_H
