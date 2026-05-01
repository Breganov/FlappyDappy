#include "start_match_use_case.h"

StartMatchUseCase::StartMatchUseCase(SessionService &sessions)
    : sessions_(sessions) {}

void StartMatchUseCase::Execute(const SessionId &session_id) {
  const auto &session = sessions_.FindSession(session_id);
  if (!session) {
    return;
  }
  session->get().StartCountdown();
  session->get().StartMatch();
}
