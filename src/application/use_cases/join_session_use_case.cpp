/* Вот правильная последовательность:
   1. получить SessionId и PlayerId;
   2. найти сессию через SessionService;
   3. если сессия не найдена — сообщить об ошибке;
   4. вызвать session.AddPlayer(player_id);
   5. вызвать broadcaster.NotifyPlayerJoined(session_id, player_id).
*/

#include "join_session_use_case.h"

JoinSessionUseCase::JoinSessionUseCase(SessionService &session,
                                       ISessionBroadcaster &broadcaster)
    : session_(session), broadcaster_(broadcaster) {}

void JoinSessionUseCase::Execute(const SessionId &id,
                                 const PlayerId &player_id) {
  auto session = session_.FindSession(id);
  if (!session) {
    return;
  }
  session->get().AddPlayer(player_id);

  // TODO: временно до фазы 5 = матч стартует сразу при входе игрока.
  session->get().StartCountdown();
  session->get().StartMatch();

  broadcaster_.NotifyPlayerJoined(id, player_id);
}
