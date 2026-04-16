// session_service.h держит активные игровые сессии в памяти, добавляет, ищет и
// удаляет.
#pragma once

#include "../../domain/session/game_session.h"

#include <optional>
#include <string>
#include <unordered_map>

class SessionService {
public:
  GameSession &CreateSession();
  std::optional<std::reference_wrapper<GameSession>>
  FindSession(const SessionId &id);
  void RemoveSession(const SessionId &id);

private:
  std::unordered_map<std::string, GameSession> sessions_;
  // нужен доступ `id.ToString();`
};
