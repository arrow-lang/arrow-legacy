// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <boost/filesystem.hpp>

#include <sstream>

#include "arrow/span.hpp"

namespace fs = boost::filesystem;

namespace arrow {

std::string Position::to_string() const noexcept {
  std::stringstream st;
  st << (row + 1);
  st << '.';
  st << (column + 1);
  return st.str();
}

std::string Span::to_string() const noexcept {
  std::stringstream st;

  if (filename == "" &&
      begin.row == 0 &&
      begin.column == 0 &&
      end.row == 0 &&
      end.column == 0) {
    st << "(nil)";
  } else {
    st << fs::path(filename).string();
    st << ':';

    st << (begin.row + 1);
    st << '.';
    st << (begin.column + 1);
    st << '-';

    if (begin.row == end.row) {
      // line,column-column
      st << (end.column + 1);
    } else {
      // line,column-line,column
      st << (end.row + 1);
      st << '.';
      st << (end.column + 1);
    }
  }

  return st.str();
}

}  // namespace arrow
