// application\use_cases\game_loop_service.h
#pragma once

#include "application/use_cases/session_service.h"
#include "application/use_cases/tick_session_use_case.h"

class GameLoopService {
public:
  GameLoopService(const SessionService &sessions,
                  const TickSessionUseCase ticks);

private:
};
