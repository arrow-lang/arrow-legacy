// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_SPAN_H
#define ARROW_SPAN_H 1

#include <cstddef>
#include <string>

namespace arrow {

struct Position {
  inline Position(std::size_t row = 0, std::size_t column = 0)
      : row(row), column(column) {
  }

  inline Position operator +(std::size_t offset) {
    return Position(row, column + offset);
  }

  inline Position operator -(std::size_t offset) {
    return Position(row, column + offset);
  }

  std::string to_string() const noexcept;

  std::size_t row;
  std::size_t column;
};

struct Span {
  inline Span(std::nullptr_t = nullptr)
      : filename(""), begin({0, 0}), end({0, 0}) {
  }

  inline Span(std::string filename, Position begin, Position end)
      : filename(filename), begin(begin), end(end) {
  }

  inline Span(std::string filename, Position begin, std::size_t offset)
      : filename(filename), begin(begin), end(begin + offset) {
  }

  Span(const Span& other) noexcept
      : filename(other.filename),
        begin(other.begin),
        end(other.end) {
  }

  Span(Span&& other) noexcept {
    swap(*this, other);
  }

  friend void swap(Span& a, Span& b) noexcept {
    using std::swap;

    swap(a.filename, b.filename);
    swap(a.begin, b.begin);
    swap(a.end, b.end);
  }

  Span& operator =(Span other) noexcept {
    swap(*this, other);
    return *this;
  }

  std::string to_string() const noexcept;

  /// Creates a new span that takes the begin position of `this` and the
  /// end position of `other`.
  inline Span extend(const Span& other) {
    return Span(filename, begin, other.end);
  }

  std::string filename;
  Position begin;
  Position end;
};

}  // namespace arrow

#endif  // ARROW_SPAN_H
