// Copyright (c) 2014-2015 Ryan Leckey, All Rights Reserved.

// Distributed under the MIT License
// See accompanying file LICENSE

#include <cstdio>
#include <cstdlib>

#include "arrow/log.hpp"

using arrow::Log;

Log::Log() : _counters() {
}

Log& Log::get() noexcept {
  static Log instance;
  return instance;
}

void Log::error(const char* format, ...) {
  va_list args;
  va_start(args, format);
  this->error(format, args);
  va_end(args);
}

void Log::error(arrow::Span span, const char* format, ...) {
  va_list args;
  va_start(args, format);
  this->error(span, format, args);
  va_end(args);
}

void Log::error(const char* format, va_list arguments) {
  _counters["error"] += 1;

  std::fprintf(stderr, "\x1b[0;37m%s", "arrow: ");
  std::fprintf(stderr, "\x1b[0;31m%s\x1b[1;37m", "error: ");
  std::vfprintf(stderr, format, arguments);
  std::fprintf(stderr, "\x1b[0m\n");
}

void Log::error(arrow::Span span, const char* format, va_list arguments) {
  _counters["error"] += 1;

  std::fprintf(stderr, "\x1b[0;37m%s%s", span.to_string().c_str(), ": ");
  std::fprintf(stderr, "\x1b[0;31m%s\x1b[1;37m", "error: ");
  std::vfprintf(stderr, format, arguments);
  std::fprintf(stderr, "\x1b[0m\n");
}

void Log::warning(const char* format, ...) {
  va_list args;
  va_start(args, format);
  this->warning(format, args);
  va_end(args);
}

void Log::warning(arrow::Span span, const char* format, ...) {
  va_list args;
  va_start(args, format);
  this->warning(span, format, args);
  va_end(args);
}

void Log::warning(const char* format, va_list arguments) {
  _counters["warning"] += 1;

  std::fprintf(stderr, "\x1b[0;37m%s", "arrow: ");
  std::fprintf(stderr, "\x1b[0;33m%s\x1b[1;37m", "warning: ");
  std::vfprintf(stderr, format, arguments);
  std::fprintf(stderr, "\x1b[0m\n");
}

void Log::warning(arrow::Span span, const char* format, va_list arguments) {
  _counters["warning"] += 1;

  std::fprintf(stderr, "\x1b[0;37m%s%s", span.to_string().c_str(), ": ");
  std::fprintf(stderr, "\x1b[0;33m%s\x1b[1;37m", "warning: ");
  std::vfprintf(stderr, format, arguments);
  std::fprintf(stderr, "\x1b[0m\n");
}

void Log::info(const char* format, ...) {
  va_list args;
  va_start(args, format);
  this->info(format, args);
  va_end(args);
}

void Log::info(arrow::Span span, const char* format, ...) {
  va_list args;
  va_start(args, format);
  this->info(span, format, args);
  va_end(args);
}

void Log::info(const char* format, va_list arguments) {
  std::fprintf(stderr, "\x1b[0;37m%s", "arrow: ");
  std::fprintf(stderr, "\x1b[0;36m%s\x1b[1;37m", "info: ");
  std::vfprintf(stderr, format, arguments);
  std::fprintf(stderr, "\x1b[0m\n");
}

void Log::info(arrow::Span span, const char* format, va_list arguments) {
  std::fprintf(stderr, "\x1b[0;37m%s%s", span.to_string().c_str(), ": ");
  std::fprintf(stderr, "\x1b[0;36m%s\x1b[1;37m", "info: ");
  std::vfprintf(stderr, format, arguments);
  std::fprintf(stderr, "\x1b[0m\n");
}

void Log::trace(const char* format, ...) {
  va_list args;
  va_start(args, format);
  this->trace(format, args);
  va_end(args);
}

void Log::trace(arrow::Span span, const char* format, ...) {
  va_list args;
  va_start(args, format);
  this->trace(span, format, args);
  va_end(args);
}

void Log::trace(const char* format, va_list arguments) {
  if (!LOG_TRACE) return;

  std::fprintf(stderr, "\x1b[0;37m%s", "arrow: ");
  std::fprintf(stderr, "\x1b[1;30m%s\x1b[1;37m", "trace: ");
  std::vfprintf(stderr, format, arguments);
  std::fprintf(stderr, "\x1b[0m\n");
}

void Log::trace(arrow::Span span, const char* format, va_list arguments) {
  if (!LOG_TRACE) return;

  std::fprintf(stderr, "\x1b[0;37m%s%s", span.to_string().c_str(), ": ");
  std::fprintf(stderr, "\x1b[1;30m%s\x1b[1;37m", "trace: ");
  std::vfprintf(stderr, format, arguments);
  std::fprintf(stderr, "\x1b[0m\n");
}

unsigned Log::count(const char* level) {
  return _counters[level];
}
