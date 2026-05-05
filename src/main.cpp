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
  GameConfig game_config{PhysicsConfig{}};

  CreateSessionUseCase session(ids, sessions, game_config);
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
