// src/infrastructure/net/websocket_session_broadcaster.h
#pragma once

#include "application/ports/session_broadcaster.h"
#include "domain/game/world_snapshot.h"
#include "domain/match/match_result.h"
#include "infrastructure/net/connection_registry.h"
#include "infrastructure/net/websocket_message_serializer.h"
#include "infrastructure/net/websocket_session.h"

// The real ISAessionBroadcaster: serializes domain objects to JSON and
// fans them out to every socket registered for that game session.
class WebSocketSessionBroadcaster : public ISessionBroadcaster {
public:
  WebSocketSessionBroadcaster(ConnectionRegistry &registry,
                              WebSocketMessageSerializer &serializer);

  void BroadcastSnapshot(const SessionId &session_id,
                         const WorldSnapshot &snapshot) override;

  void BroadcastMatchFinished(const SessionId &session_id,
                              const MatchResult &result) override;

  void NotifyPlayerJoined(const SessionId &session_id,
                          const PlayerId &player_id) override;

private:
  ConnectionRegistry &registry_;
  WebSocketMessageSerializer &serializer_;
};
