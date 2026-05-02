// application/use_cases/finish_match_use_case.cpp
#include "finish_match_use_case.h"
#include "domain/session/session_id.h"

FinishMatchUseCase::FinishMatchUseCase(SessionService &sessions)
    : sessions_(sessions) {}

void FinishMatchUseCase::Execute(const SessionId &session_id) {
  auto session = sessions_.FindSession(session_id);
  if (!session) {
    return;
  }

  if (!session->get().IsFinished()) {
    return;
  }

  sessions_.RemoveSession(session_id);
}
