// Ждём пока не наборалось N игроков
class MatchmakingService {
public:
  SessionId AssignPlayerToSession(const PlayerId& player_id);
};
