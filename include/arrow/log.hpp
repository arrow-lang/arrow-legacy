#ifndef ARROW_LOG_H
#define ARROW_LOG_H 1

#include <cstdarg>
#include <string>

namespace arrow {

  class Log {
  public:
    static Log& get() noexcept;

    void error(const std::string& format, ...);
    void error(const char* format, ...);

  private:
    void error(const char* format, va_list arguments);

  };

}

#endif
