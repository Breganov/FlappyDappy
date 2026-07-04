// infrastructure/net/connections_registry.h
#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class WebSocketSession;

// Maps game session ids to the live socket connections of their players.
// Holds weak_ptr so the registry never keeps a dead connection alive;
// expired entries are swept lazily on each broadcas.
class ConnectionRegistry {
public:
  void Register(const std::string &session_id,
                std::weak_ptr<WebSocketSession> connection);

  void ForEachInSession(const std::string &session_id,
                        const std::function<void(WebSocketSession &)> &fn);

private:
  std::unordered_map<std::string, std::vector<std::weak_ptr<WebSocketSession>>>
      connections_;
};
