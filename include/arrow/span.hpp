#ifndef ARROW_SPAN_H
#define ARROW_SPAN_H 1

#include <string>

namespace arrow {

  struct Position {
    Position(unsigned row, unsigned column);

    Position operator +(unsigned offset);

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

}

#endif // ARROW_SPAN_H
