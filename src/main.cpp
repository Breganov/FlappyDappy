#include "domain/session/game_session.h"
#include "infrastructure/logging/console_logger.h"

int main() {
  ConsoleLogger logger(LogLevel::Debug);
  logger.Info("FlappyDappy server starting");

  GameSession gs;
  gs.AddPlayer(PlayerId("First player"));
  gs.AddPlayer(PlayerId("Second player"));
  // create services, sessions, etc.

  logger.Info("Server initialized");

  // TODO
  // - session created;
  // - palyer added;
  // - match started;
  // - palyer died;
  // - match finished;

  return 0;
}
