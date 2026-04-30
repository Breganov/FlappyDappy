// session_service.h держит активные игровые сессии в памяти, добавляет, ищет и
// удаляет.
#pragma once

#include "domain/session/game_session.h"
#include "domain/session/session_id.h"

#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

class SessionService {
public:
  GameSession &CreateSession(SessionId id, std::uint32_t seed, double gravity,
                             double jump_veloctiy, double scroll_speed);

  std::optional<std::reference_wrapper<GameSession>>
  FindSession(const SessionId &id);

  void RemoveSession(const SessionId &id);

  std::vector<std::reference_wrapper<GameSession>> GetAllSessions();

private:
  std::unordered_map<std::string, GameSession> sessions_;
};
