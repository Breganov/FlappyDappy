// application\use_cases\finish_match_use_case.cpp
#include "finish_match_use_case.h"
#include "domain/session/session_id.h"

FinishMatchUseCase::FinishMatchUseCase(ISessionBroadcaster &broadcaster,
                                       SessionService &sessions)
    : broadcaster_(broadcaster), sessions_(sessions) {}

void FinishMatchUseCase::Execute(const SessionId &session_id) {
  const auto &session = sessions_.FindSession(session_id);
  if (!session) {
    return;
  }
  if (session->get().IsFinished()) {
    const auto &match_result = session->get().BuildResult();
    broadcaster_.BroadcastMatchFinished(session_id, match_result);
    sessions_.RemoveSession(session_id);
  }
}
