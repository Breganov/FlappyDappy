#include "domain/session/game_session.h"
#include "infrastructure/logging/console_logger.h"

int main() {
  ConsoleLogger logger(LogLevel::Debug);
  logger.Info("FlappyDappy server starting");

  GameSession gs(SessionId("First id"), 42u, 900.0f, -300.0f);
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
