// application\use_cases\create_session_use_case.h
#pragma once

#include "application/ports/id_generator.h"
#include "session_service.h"

class CreateSessionUseCase {
public:
  CreateSessionUseCase(IIdGenerator &ids, SessionService &sessions);

  SessionId Execute();

private:
  IIdGenerator &ids_;
  SessionService &sessions_;
};
