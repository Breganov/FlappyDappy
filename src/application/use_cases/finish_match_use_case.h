// application\use_cases\finish_match_use_case.h
#pragma once
#include "application/ports/session_broadcaster.h"
#include "application/use_cases/session_service.h"
#include "domain/session/session_id.h"

class FinishMatchUseCase {
public:
  FinishMatchUseCase(
      // IUnitOfWorkFactory &uow_factory, // Не написал class UnitOfWorkFactory
      ISessionBroadcaster &broadcaster, SessionService &sessions);

  void Execute(const SessionId &session_id);

private:
  ISessionBroadcaster &broadcaster_;
  SessionService &sessions_;
};
