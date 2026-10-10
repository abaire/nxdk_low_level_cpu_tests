#include "sse_tests/sse_test_common.h"

#include <hal/debug.h>
#include <windows.h>

#include <cstdarg>
#include <cstdio>
#include <vector>

#include "logger.h"
#include "printf/printf.h"

static std::deque<std::string> g_on_screen_log;

const std::deque<std::string>& GetOnScreenLog() { return g_on_screen_log; }

void ClearOnScreenLog() { g_on_screen_log.clear(); }

void LogMsg(const char* fmt, ...) {
  char buf[512];
  va_list args;
  va_start(args, fmt);
  vsnprintf_(buf, sizeof(buf), fmt, args);
  va_end(args);

  DbgPrint("%s", buf);

  if (Logger::IsInitialized()) {
    Logger::Log() << buf;
    Logger::Log().flush();
  }

  std::string s(buf);
  size_t start = 0;
  while (start < s.size()) {
    size_t end = s.find('\n', start);
    if (end == std::string::npos) {
      if (start < s.size()) {
        g_on_screen_log.push_back(s.substr(start));
      }
      break;
    }
    g_on_screen_log.push_back(s.substr(start, end - start));
    start = end + 1;
  }

  while (g_on_screen_log.size() > 20) {
    g_on_screen_log.pop_front();
  }
}
