#include "console_session_broadcaster.h"
#include "domain/match/match_result.h"
#include "domain/session/session_id.h"
#include "infrastructure/logging/logger.h"

#include <string>

ConsoleSessionBroadcaster::ConsoleSessionBroadcaster(ILogger &logger)
    : logger_(logger) {}

void ConsoleSessionBroadcaster::BroadcastSnapshot(
    const SessionId &session_id, const WorldSnapshot &snapshot) {
  logger_.Info(std::string("Broadcast snapshot for session: ") +
               session_id.ToString() +
               ", tick=" + std::to_string(snapshot.tick));
}

void ConsoleSessionBroadcaster::BroadcastMatchFinished(
    const SessionId &session_id, const MatchResult &result) {
  logger_.Info(std::string("Match finished for session: ") +
               session_id.ToString() +
               ", players ranked=" + std::to_string(result.rankings.size()));
}

void ConsoleSessionBroadcaster::NotifyPlayerJoined(const SessionId &session_id,
                                                   const PlayerId &player_id) {
  logger_.Info(std::string("Player join session: ") + session_id.ToString() +
               ", player_id=" + player_id.ToString());
}
