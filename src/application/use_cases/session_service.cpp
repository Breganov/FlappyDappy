#include "session_service.h"

GameSession &SessionService::CreateSession(SessionId id, std::uint32_t seed,
                                           double gravity, double jump_veloctiy,
                                           double scroll_speed) {
  auto [it, inserted] = sessions_.emplace(
      id.ToString(),
      GameSession(std::move(id), seed, gravity, jump_veloctiy, scroll_speed));
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
