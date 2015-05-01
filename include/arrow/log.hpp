// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#ifndef ARROW_LOG_H
#define ARROW_LOG_H 1

#include <cstdarg>
#include <string>
#include <map>

#include "arrow/span.hpp"

namespace arrow {

class Log {
 public:
  static Log& get() noexcept;

  Log();

  // FIXME: This shouldn't be so many functions

  unsigned count(const char* level);

  void error(const char* format, ...);
  void error(Span span, const char* format, ...);

  void warning(const char* format, ...);
  void warning(Span span, const char* format, ...);

 private:
  void error(const char* format, va_list arguments);
  void error(Span, const char* format, va_list arguments);

  void warning(const char* format, va_list arguments);
  void warning(Span, const char* format, va_list arguments);

  std::map<std::string, unsigned> _counters;
};

}  // namespace arrow

#endif  // ARROW_LOG_H
