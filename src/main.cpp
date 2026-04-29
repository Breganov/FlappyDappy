// main.cpp
#include "domain/game/collision_service.h"
#include "domain/session/game_session.h"
#include "infrastructure/logging/console_logger.h"

#include <chrono>
#include <iostream>
#include <string>

int main() {
  ConsoleLogger logger(LogLevel::Debug);
  logger.Info("FlappyDappy server starting");
  SessionId sid("test-session");
  const std::uint32_t seed = 42u;
  double gravity = 900.0f;
  const double jump_velocity = -300.0f;
  const double scroll_speed = 120.0f;
  CollisionService collision_service;

  GameSession gs(sid, seed, gravity, jump_velocity, scroll_speed,
                 collision_service);

  gs.AddPlayer(PlayerId("p1"));
  gs.AddPlayer(PlayerId("p2"));
  for (const auto &player : gs.GetPlayers()) {
    logger.Info("Added player: " + player.GetPlayerId().ToString());
  }

  gs.StartCountdown();
  if (gs.GetState() != SessionState::Countdown) {
    logger.Error("GameSession.StartCountdown didn't started.");
  }

  gs.StartMatch();
  if (gs.GetState() != SessionState::InProgress) {
    logger.Error("GameSession.StartMatch didn't started.");
  }

  logger.Info("Server initialized");

  logger.Info("Match Started");
  int ticks_done = 0;
  constexpr auto tick_duration = std::chrono::milliseconds(16);

  for (int i = 0; i < 500 && !gs.IsFinished(); ++i) {
    if (gs.IsFinished()) {
      logger.Info("Match finished before tick: " + std::to_string(i + 1));
      break;
    }

    if (i % 50 == 0) {
      InputCommand cmd = {PlayerId("p1"), InputType::Jump, 0u};
      gs.EnqueueInput(cmd);
    }

    gs.Tick(std::chrono::milliseconds(tick_duration));
    ++ticks_done;

    if ((i + 1) % 10 == 0 || gs.IsFinished()) {
      WorldSnapshot ws = gs.BuildSnapshot();
      logger.Info(std::to_string(ws.tick) + " ticks");
      std::cout << ws;
    }
  }

  logger.Info("Loop finished.");
  logger.Info("Ticks done: " + std::to_string(ticks_done));
  logger.Info("IsFinished: " + std::to_string(gs.IsFinished()));

  return 0;
}
