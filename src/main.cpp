// main.cpp
#include "application/ports/simple_id_generator.h"
#include "application/use_cases/create_session_use_case.h"
#include "application/use_cases/finish_match_use_case.h"
#include "application/use_cases/join_session_use_case.h"
#include "application/use_cases/session_service.h"
#include "application/use_cases/start_match_use_case.h"
#include "application/use_cases/submit_input_use_case.h"
#include "application/use_cases/tick_session_use_case.h"
#include "domain/player/player_id.h"
#include "domain/session/input_command.h"
#include "infrastructure/logging/console_logger.h"
#include "infrastructure/logging/console_session_broadcaster.h"
#include <chrono>
#include <string>

int main() {
  ConsoleLogger logger;
  logger.Info("Starting a new server.");

  SimpleIdGenerator ids;
  SessionService sessions;
  ConsoleSessionBroadcaster broadcaster(logger);

  CreateSessionUseCase session(ids, sessions);
  SessionId id = session.Execute();
  auto create_session = sessions.FindSession(id);
  if (!create_session) {
    logger.Error("Session was not found right after creation: " +
                 id.ToString());
    return 1;
  }

  logger.Info("Session found right after creation.");
  logger.Info("IDs generated.");
  logger.Info("Sessions created.");

  JoinSessionUseCase join_session_use_case(sessions, broadcaster);

  PlayerId player_id("player-1");
  join_session_use_case.Execute(id, player_id);
  logger.Info("Player " + player_id.ToString() + " added.");

  StartMatchUseCase start_match_use_case(sessions);
  start_match_use_case.Execute(id);
  logger.Info("Match " + id.ToString() + " started.");

  SubmitInputUseCase submit_input_use_case(sessions);
  TickSessionUseCase tick_session_use_case(sessions, broadcaster);
  FinishMatchUseCase finish_match_use_case(sessions);

  for (int i = 0; i < 300; ++i) {
    auto session_instance = sessions.FindSession(id);
    if (!session_instance) {
      logger.Info("Session wasn't found.");
      break;
    }

    if (session_instance->get().IsFinished()) {
      logger.Info("Session is finished.");
      break;
    }

    logger.Info("tick=" + std::to_string(i));
    if ((i + 1) % 20 == 0) {
      submit_input_use_case.Execute(id,
                                    InputCommand{player_id, InputType::Jump});
      logger.Info("Jump by " + player_id.ToString() +
                  " sent at tick=" + std::to_string(i));
    }

    tick_session_use_case.Execute(id, std::chrono::milliseconds(16));
  }

  finish_match_use_case.Execute(id);
  logger.Info("Session " + id.ToString() + " finished.");
  return 0;
}

// Old working Main CPP
// int main() {
//   ConsoleLogger logger(LogLevel::Debug);
//   logger.Info("FlappyDappy server starting");
//   SessionId sid("test-session");
//   const std::uint32_t seed = 42u;
//   double gravity = 900.0f;
//   const double jump_velocity = -300.0f;
//   const double scroll_speed = 120.0f;
//   CollisionService collision_service;
//
//   GameSession gs(sid, seed, gravity, jump_velocity, scroll_speed,
//                  collision_service);
//
//   gs.AddPlayer(PlayerId("p1"));
//   gs.AddPlayer(PlayerId("p2"));
//   for (const auto &player : gs.GetPlayers()) {
//     logger.Info("Added player: " + player.GetPlayerId().ToString());
//   }
//
//   gs.StartCountdown();
//   if (gs.GetState() != SessionState::Countdown) {
//     logger.Error("GameSession.StartCountdown didn't started.");
//   }
//
//   gs.StartMatch();
//   if (gs.GetState() != SessionState::InProgress) {
//     logger.Error("GameSession.StartMatch didn't started.");
//   }
//
//   logger.Info("Server initialized");
//
//   logger.Info("Match Started");
//   int ticks_done = 0;
//   constexpr auto tick_duration = std::chrono::milliseconds(16);
//
//   for (int i = 0; i < 500 && !gs.IsFinished(); ++i) {
//     if (gs.IsFinished()) {
//       logger.Info("Match finished before tick: " + std::to_string(i + 1));
//       break;
//     }
//
//     if (i % 50 == 0) {
//       InputCommand cmd = {PlayerId("p1"), InputType::Jump, 0u};
//       gs.EnqueueInput(cmd);
//     }
//
//     gs.Tick(std::chrono::milliseconds(tick_duration));
//     ++ticks_done;
//
//     if ((i + 1) % 10 == 0 || gs.IsFinished()) {
//       WorldSnapshot ws = gs.BuildSnapshot();
//       logger.Info(std::to_string(ws.tick) + " ticks");
//       std::cout << ws;
//     }
//   }
//
//   logger.Info("Loop finished.");
//   logger.Info("Ticks done: " + std::to_string(ticks_done));
//   logger.Info("IsFinished: " + std::to_string(gs.IsFinished()));
//
//   return 0;
// }
