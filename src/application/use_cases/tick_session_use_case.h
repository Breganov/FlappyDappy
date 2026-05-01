#pragma once

#include "application/ports/session_broadcaster.h"
#include "application/use_cases/session_service.h"
#include "domain/session/session_id.h"

class TickSessionUseCase {
public:
  explicit TickSessionUseCase(SessionService &sessions,
                              ISessionBroadcaster &broadcast_);
  void Execute(const SessionId &session_id, std::chrono::milliseconds delta);

private:
  SessionService &sessions_;
  ISessionBroadcaster &broadcast_;
};
