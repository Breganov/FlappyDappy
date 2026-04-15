#pragma once

#include "logger.h"
#include <string_view>

class ConsoleLogger : public ILogger {
public:
  explicit ConsoleLogger(LogLevel min_level = LogLevel::Info);

  void Log(LogLevel level, std::string_view message) override;

private:
  LogLevel min_level_;
};
