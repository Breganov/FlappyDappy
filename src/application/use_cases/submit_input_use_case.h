#pragma once

#include "application/use_cases/session_service.h"
#include "domain/session/input_command.h"

class SubmitInputUseCase {
public:
  explicit SubmitInputUseCase(SessionService &sessions) : sessions_(sessions) {}

  bool Execute(const SessionId &session_id, const InputCommand &command) {
    auto session = sessions_.FindSession(session_id);
    if (!session) {
      return false;
    }

    session->get().EnqueueInput(command);
    return true;
  }

private:
  SessionService &sessions_;
};
