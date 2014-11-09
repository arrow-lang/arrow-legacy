#include <sstream>
#include "arrow/span.hpp"

using arrow::Position;
using arrow::Span;

Position::Position(unsigned row, unsigned column)
  : row(row), column(column) {
}

Span::Span(const std::string& filename, Position begin, Position end)
  : filename(filename), begin(begin), end(end) {
}

Span::Span(std::string&& filename, Position begin, Position end)
  : filename(filename), begin(begin), end(end) {
}

std::string Span::to_string() const {
  std::stringstream fmt;

  fmt << (this->begin.row + 1);
  fmt << ',';
  fmt << (this->begin.column + 1);
  fmt << '-';

  if (this->begin.row == this->end.row) {
    // line,column-column
    fmt << (this->end.column + 1);
  } else {
    // line,column-line,column
    fmt << (this->end.row + 1);
    fmt << ',';
    fmt << (this->end.column + 1);
  }

  return fmt.str();
}
