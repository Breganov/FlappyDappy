#include "infrastructure/logging/console_logger.h"

int main() {
  ConsoleLogger logger(LogLevel::Debug);
  logger.Info("FlappyDappy server starting");

  // create services, sessions, etc.
  
  logger.Info("Server initialized");
  return 0;
}
