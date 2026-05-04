// application/use_cases/create_session_use_case.h
#pragma once

#include "application/ports/id_generator.h"
#include "application/use_cases/game_config.h"
#include "session_service.h"

class CreateSessionUseCase {
public:
  CreateSessionUseCase(IIdGenerator &ids, SessionService &sessions,
                       GameConfig &config);

  SessionId Execute();

private:
  IIdGenerator &ids_;
  SessionService &sessions_;
  GameConfig &config_;
};
