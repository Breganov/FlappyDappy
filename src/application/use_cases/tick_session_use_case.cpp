#include "tick_session_use_case.h"

TickSessionUseCase::TickSessionUseCase(SessionService &sessions,
                                       ISessionBroadcaster &broadcast)
    : sessions_(sessions), broadcast_(broadcast) {}

void TickSessionUseCase::Execute(const SessionId &session_id,
                                 std::chrono::milliseconds delta) {
  auto session = sessions_.FindSession(session_id);
  if (!session) {
    return;
  }

  session->get().Tick(delta);

  auto snapshot = session->get().BuildSnapshot();
  broadcast_.BroadcastSnapshot(session_id, snapshot);

  if (session->get().IsFinished()) {
    auto result = session->get().BuildResult();
    broadcast_.BroadcastMatchFinished(session_id, result);
  }
}
