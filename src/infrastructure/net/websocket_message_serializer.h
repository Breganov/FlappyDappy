// infrastructure/net/websocket_message_serializer.h
#pragma once

#include "domain/game/world_snapshot.h"
#include "domain/match/match_result.h"

#include <string>

class WebSocketMessageSerializer {
public:
  std::string SerializeSnapshot(const WorldSnapshot &snapshot) const;
  std::string SerializeMatchResult(const MatchResult &result) const;
};
