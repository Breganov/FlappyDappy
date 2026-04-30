#pragma once

#include "application/ports/session_broadcaster.h"
#include "domain/player/player_id.h"
#include "domain/session/session_id.h"
#include "session_service.h"

class JoinSessionUseCase {
public:
  JoinSessionUseCase(SessionService &session, ISessionBroadcaster &broadcaster);

  void Execute(const SessionId &id, const PlayerId &player_id);

private:
  SessionService &session_;
  ISessionBroadcaster &broadcaster_;
};
