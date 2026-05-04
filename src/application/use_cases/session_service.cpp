// application/use_cases/session_service.cpp
#include "session_service.h"

GameSession &SessionService::CreateSession(SessionId id, std::uint32_t seed,
                                           double gravity, double jump_velocity,
                                           double scroll_speed) {
  const std::string key = id.ToString();
  auto [it, inserted] =
      sessions_.emplace(key, GameSession(std::move(id), seed, gravity,
                                         jump_velocity, scroll_speed));
  return it->second;
}

std::optional<std::reference_wrapper<GameSession>>
SessionService::FindSession(const SessionId &id) {
  auto it = sessions_.find(id.ToString());
  if (it == sessions_.end()) {
    return std::nullopt;
  }
  return it->second;
}
void SessionService::RemoveSession(const SessionId &id) {
  sessions_.erase(id.ToString());
}

std::vector<std::reference_wrapper<GameSession>>
SessionService::GetAllSessions() {
  std::vector<std::reference_wrapper<GameSession>> result;
  result.reserve(sessions_.size());
  for (auto &[_, session] : sessions_) {
    result.push_back(session);
  }
  return result;
}
