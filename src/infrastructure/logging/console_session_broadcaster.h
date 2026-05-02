// infrastructure\logging\console_session_broadcaster.h
#pragma once

#include "application/ports/session_broadcaster.h"
#include "domain/match/match_result.h"
#include "infrastructure/logging/logger.h"

class ConsoleSessionBroadcaster : public ISessionBroadcaster {
public:
  explicit ConsoleSessionBroadcaster(ILogger &logger);

  void BroadcastSnapshot(const SessionId &session_id,
                         const WorldSnapshot &snapshot) override;

  void BroadcastMatchFinished(const SessionId &session_id,
                              const MatchResult &result) override;

  void NotifyPlayerJoined(const SessionId &session_id,
                          const PlayerId &player_id) override;

private:
  ILogger &logger_;
};
