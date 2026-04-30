#include "tick_session_use_case.h"
#include "domain/game/world_snapshot.h"

TickSessionUseCase::TickSessionUseCase(SessionService &sessions,
                                       ISessionBroadcaster &broadcast)
    : sessions_(sessions), broadcast_(broadcast) {}

void TickSessionUseCase::Execute(const SessionId &session_id,
                                 std::chrono::milliseconds delta) {
  auto sessions = sessions_.FindSession(session_id);
  if (!sessions) {
    return;
  }

  sessions->get().Tick(delta);

  auto snapshot = sessions->get().BuildSnapshot();
  broadcast_.BroadcastSnapshot(session_id, snapshot);

  if (sessions->get().IsFinished()) {
    auto result = sessions->get().BuildResult();
    broadcast_.BroadcastMatchFinished(session_id, result);
  }
}
