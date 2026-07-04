// src/infrastructure/net/websocket_session_broadcaster.cpp
#include "websocket_session_broadcaster.h"

#include "domain/game/world_snapshot.h"
#include "infrastructure/net/connection_registry.h"
#include "websocket_session.h"

WebSocketSessionBroadcaster::WebSocketSessionBroadcaster(
    ConnectionRegistry &registry, WebSocketMessageSerializer &serializer)
    : registry_(registry), serializer_(serializer) {}

void WebSocketSessionBroadcaster::BroadcastSnapshot(
    const SessionId &session_id, const WorldSnapshot &snapshot) {
  const std::string json = serializer_.SerializeSnapshot(snapshot);
  registry_.ForEachInSession(session_id.ToString(),
                             [&](WebSocketSession &s) { s.Send(json); });
}

void WebSocketSessionBroadcaster::BroadcastMatchFinished(
    const SessionId &session_id, const MatchResult &result) {
  const std::string json = serializer_.SerializeMatchResult(result);
  registry_.ForEachInSession(session_id.ToString(),
                             [&](WebSocketSession &s) { s.Send(json); });
}

void WebSocketSessionBroadcaster::NotifyPlayerJoined(
    const SessionId &session_id, const PlayerId &player_id) {
  const std::string json = R"({"type":"player_joined","player_id":")" +
                           player_id.ToString() + R"("})";
  registry_.ForEachInSession(session_id.ToString(),
                             [&](WebSocketSession &s) { s.Send(json); });
}
