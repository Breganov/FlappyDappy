// application\use_cases\mark_ready_use_case.h
#pragma once

#include "application/use_cases/session_service.h"

class MarkReadyUseCases {
public:
  MarkReadyUseCases(SessionService &sessions);
  void Execute(const SessionId &session_id, const PlayerId &player_id);

private:
  SessionService &sessions_;
};
