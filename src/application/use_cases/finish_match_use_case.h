// application/use_cases/finish_match_use_case.h
#pragma once

#include "application/use_cases/session_service.h"
#include "domain/session/session_id.h"

class FinishMatchUseCase {
public:
  FinishMatchUseCase(SessionService &sessions);

  void Execute(const SessionId &session_id);

private:
  SessionService &sessions_;
};
