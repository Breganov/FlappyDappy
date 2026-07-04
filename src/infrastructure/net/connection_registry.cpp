// infrastructure/net/connections_registry.cpp
#include "connection_registry.h"

#include "websocket_session.h"

void ConnectionRegistry::Register(const std::string &session_id,
                                  std::weak_ptr<WebSocketSession> connection) {
  connections_[session_id].push_back(std::move(connection));
}

void ConnectionRegistry::ForEachInSession(
    const std::string &session_id,
    const std::function<void(WebSocketSession &)> &fn) {
  auto it = connections_.find(session_id);
  if (it == connections_.end()) {
    return;
  }

  auto &list = it->second;
  // Visit live connections; compact away the ones that died.
  std::size_t write_index = 0;
  for (auto &weak : list) {
    if (auto strong = weak.lock()) {
      fn(*strong);
      list[write_index++] = std::move(weak);
    }
  }
  list.resize(write_index);

  if (list.empty()) {
    connections_.erase(it);
  }
}
