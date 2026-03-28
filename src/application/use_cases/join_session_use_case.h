// Найти и создать сессию
// Добавить игрока
// Уведомить остальных
class JoinSessionUseCase {
public:
  JoinSessionUseCase(SessionService& session, ISessionBroadcaster& broadcaster);

  SessionId Execute(const PlayerId& player_id);
};
