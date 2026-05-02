// application\use_cases\game_loop_service.h
#pragma once

#include "application/use_cases/finish_match_use_case.h"
#include "application/use_cases/session_service.h"
#include "application/use_cases/tick_session_use_case.h"

#include <chrono>

class GameLoopService {
public:
  GameLoopService(SessionService &sessions, TickSessionUseCase &ticks,
                  FinishMatchUseCase &finish);
  void Execute(std::chrono::milliseconds delta);

private:
  SessionService &sessions_;
  TickSessionUseCase &ticks_;
  FinishMatchUseCase &finish_;
};
