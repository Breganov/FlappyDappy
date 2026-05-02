// application\use_cases\game_loop_service.cpp
#include "game_loop_service.h"

GameLoopService::GameLoopService(SessionService &sessions,
                                 TickSessionUseCase &ticks,
                                 FinishMatchUseCase &finish)
    : sessions_(sessions), ticks_(ticks), finish_(finish) {};

void GameLoopService::Execute(std::chrono::milliseconds delta) {
  for (const auto &session : sessions_.GetAllSessions()) {
    ticks_.Execute(session.get().GetId(), delta);
    if (session.get().IsFinished()) {
      finish_.Execute(session.get().GetId());
    }
  }
}
