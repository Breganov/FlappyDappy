#include "console_logger.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

const char *ToString(LogLevel level) {
  switch (level) {
  case LogLevel::Trace:
    return "TRACE";
  case LogLevel::Debug:
    return "DEBUG";
  case LogLevel::Info:
    return "INFO";
  case LogLevel::Warn:
    return "WARN";
  case LogLevel::Error:
    return "ERROR";
  }
  return "UNKNOWN";
}

bool ShouldLog(LogLevel current, LogLevel min_level) {
  return static_cast<int>(current) >= static_cast<int>(min_level);
}

ConsoleLogger::ConsoleLogger(LogLevel min_level) : min_level_(min_level) {}

void ConsoleLogger::Log(LogLevel level, std::string_view message) {
  if (!ShouldLog(level, min_level_)) {
    return;
  }

  const auto now = std::chrono::system_clock::now();
  const auto time = std::chrono::system_clock::to_time_t(now);

  std::tm local_tm{};
#ifdef _WIN32
  localtime_s(&local_tm, &time);
#else
  localtime_r(&time, &local_tm);
#endif

  std::cout << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S") << " ["
            << ToString(level) << "] " << message << '\n';
}
