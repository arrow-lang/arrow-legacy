#ifndef ARROW_LOG_H
#define ARROW_LOG_H 1

#include <cstdarg>
#include <string>
#include <map>

namespace arrow {

  class Log {
  public:
    static Log& get() noexcept;

    unsigned count(const std::string& level);

    void error(const std::string& format, ...);
    void error(const char* format, ...);

  private:
    void error(const char* format, va_list arguments);

    std::map<std::string, unsigned> _counters;

  };

}

#endif
