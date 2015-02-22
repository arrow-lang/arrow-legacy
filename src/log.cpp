#include <cstdio>
#include <cstdlib>
#include "arrow/log.hpp"

using arrow::Log;

Log& Log::get() noexcept {
  static Log instance;
  return instance;
}

void Log::error(const std::string& format, ...)
{
  va_list args;
  va_start(args, format);
  this->error(format.c_str(), args);
  va_end(args);
}

void Log::error(const char* format, ...)
{
  va_list args;
  va_start(args, format);
  this->error(format, args);
  va_end(args);
}

void Log::error(const char* format, va_list arguments)
{
  std::fprintf(stderr, "\x1b[1;37m%s", "arrow: ");
  std::fprintf(stderr, "\x1b[1;31m%s\x1b[1;37m", "error: ");
  std::vfprintf(stderr, format, arguments);
  std::fprintf(stderr, "\x1b[0m\n");
}
