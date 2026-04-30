#pragma once
#include "application/ports/session_broadcaster.h"
#include "application/use_cases/session_service.h"
#include "domain/session/session_id.h"

class FinishMatchUseCases {
public:
  FinishMatchUseCases(
      IUnitOfWorkFactory &uow_factory, // Не написал class UnitOfWorkFactory
                                       // ебать
      ISessionBroadcaster &broadcaster, SessionService &sessions);

  void Execute(const SessionId &session_id);
};
