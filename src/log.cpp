#include <cstdio>
#include <cstdlib>
#include "arrow/log.hpp"

using arrow::Log;

Log::Log()
  : _counters()
{
}

Log& Log::get() noexcept
{
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

void Log::error(arrow::Span span, const std::string& format, ...)
{
  va_list args;
  va_start(args, format);
  this->error(span, format.c_str(), args);
  va_end(args);
}

void Log::error(const char* format, va_list arguments)
{
  _counters["error"] += 1;

  std::fprintf(stderr, "\x1b[0;37m%s", "arrow: ");
  std::fprintf(stderr, "\x1b[0;31m%s\x1b[1;37m", "error: ");
  std::vfprintf(stderr, format, arguments);
  std::fprintf(stderr, "\x1b[0m\n");
}

void Log::error(arrow::Span span, const char* format, va_list arguments)
{
  _counters["error"] += 1;

  std::fprintf(stderr, "\x1b[0;37m%s%s", span.to_string().c_str(), ": ");
  std::fprintf(stderr, "\x1b[0;31m%s\x1b[1;37m", "error: ");
  std::vfprintf(stderr, format, arguments);
  std::fprintf(stderr, "\x1b[0m\n");
}

void Log::warning(const std::string& format, ...)
{
  va_list args;
  va_start(args, format);
  this->warning(format.c_str(), args);
  va_end(args);
}

void Log::warning(arrow::Span span, const std::string& format, ...)
{
  va_list args;
  va_start(args, format);
  this->warning(span, format.c_str(), args);
  va_end(args);
}

void Log::warning(const char* format, va_list arguments)
{
  _counters["warning"] += 1;

  std::fprintf(stderr, "\x1b[0;37m%s", "arrow: ");
  std::fprintf(stderr, "\x1b[0;33m%s\x1b[1;37m", "warning: ");
  std::vfprintf(stderr, format, arguments);
  std::fprintf(stderr, "\x1b[0m\n");
}

void Log::warning(arrow::Span span, const char* format, va_list arguments)
{
  _counters["warning"] += 1;

  std::fprintf(stderr, "\x1b[0;37m%s%s", span.to_string().c_str(), ": ");
  std::fprintf(stderr, "\x1b[0;33m%s\x1b[1;37m", "warning: ");
  std::vfprintf(stderr, format, arguments);
  std::fprintf(stderr, "\x1b[0m\n");
}

unsigned Log::count(const std::string& level) {
  return _counters[level];
}
