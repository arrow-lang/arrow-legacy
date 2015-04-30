#include <sstream>
#include <boost/filesystem.hpp>
#include "arrow/span.hpp"

using arrow::Position;
using arrow::Span;

namespace fs = boost::filesystem;

Position::Position(unsigned row, unsigned column)
  : row(row), column(column) {
}

Span::Span(const std::string& filename, Position begin, Position end)
  : filename(filename), begin(begin), end(end) {
}

// NOTE: Error message format comes from
//  http://www.gnu.org/prep/standards/standards.html#Errors
std::string Span::to_string() const {
  std::stringstream fmt;

  fmt << fs::path(filename).string();
  fmt << ':';

  fmt << (begin.row + 1);
  fmt << '.';
  fmt << (begin.column + 1);
  fmt << '-';

  if (begin.row == end.row) {
    // line,column-column
    fmt << (end.column + 1);
  } else {
    // line,column-line,column
    fmt << (end.row + 1);
    fmt << '.';
    fmt << (end.column + 1);
  }

  return fmt.str();
}

auto Position::operator +(unsigned offset) -> Position {
  return Position(row, column + offset);
}
