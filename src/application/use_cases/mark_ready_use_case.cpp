// application\use_cases\mark_ready_use_case.cpp
#include "mark_ready_use_case.h"

MarkReadyUseCase::MarkReadyUseCase(SessionService &sessions)
    : sessions_(sessions) {}

void MarkReadyUseCase::Execute(const SessionId &session_id,
                               const PlayerId &player_id) {
  const auto &session = sessions_.FindSession(session_id);
  if (!session) {
    return;
  }
  session->get().MarkPlayerReady(player_id);

  // возможно эта часть не нужна =========================
  // TODO: возможно вынести эту систему отдельным файлом =
  // for (const auto &player : session->get().GetPlayers()) {
  //   if (!player.IsReady()) {
  //     return;
  //   }
  // }
  // session->get().StartCountdown();
  // конец того, что не может быть не нужно ==============
}
