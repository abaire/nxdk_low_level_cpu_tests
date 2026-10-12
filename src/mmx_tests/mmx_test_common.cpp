#include "mmx_test_common.h"

#include <cstdarg>
#include <cstdio>

#include "logger.h"
#include "printf/printf.h"

static std::deque<std::string> g_on_screen_log;
static constexpr size_t kMaxOnScreenLogLines = 18;

void LogMsg(const char* fmt, ...) {
  char buf[512];
  va_list args;
  va_start(args, fmt);
  vsnprintf_(buf, sizeof(buf), fmt, args);
  va_end(args);

  DbgPrint("%s", buf);

  if (Logger::IsInitialized()) {
    Logger::Log() << buf;
  }

  std::string s(buf);
  while (!s.empty() && (s.back() == '\n' || s.back() == '\r')) {
    s.pop_back();
  }
  if (!s.empty()) {
    g_on_screen_log.push_back(s);
    if (g_on_screen_log.size() > kMaxOnScreenLogLines) {
      g_on_screen_log.pop_front();
    }
  }
}

const std::deque<std::string>& GetOnScreenLog() { return g_on_screen_log; }

void ClearOnScreenLog() { g_on_screen_log.clear(); }
