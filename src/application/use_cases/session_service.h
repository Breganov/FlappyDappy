// session_service.h держит активные игровые сессии в памяти, добавляет, ищет и удаляет.
#include <functional>
#include <string>
#include <unordered_map>

class SessionService {
public:
  GameSession& CreateSession();
  std::optional<std::reference_wrapper<GameSession>> FindSession(const Session& id);
  void RemoveSession(const Session& id);

private:
  std::unordered_map<std::string, GameSession> sessions_;
};
