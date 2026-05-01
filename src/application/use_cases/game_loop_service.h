// application\use_cases\game_loop_service.h
#pragma once

#include "application/use_cases/session_service.h"
#include "application/use_cases/tick_session_use_case.h"

#include <chrono>

class GameLoopService {
public:
  GameLoopService(SessionService &sessions, TickSessionUseCase &ticks);
  void Execute(std::chrono::milliseconds delta);

private:
  SessionService &sessions_;
  TickSessionUseCase &ticks_;
};
