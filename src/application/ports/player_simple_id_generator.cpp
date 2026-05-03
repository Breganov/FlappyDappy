//
#include "player_simple_id_generator.h"
#include "domain/session/session_id.h"

PlayerSimpleIdGenerator::PlayerSimpleIdGenerator(std::string prefix)
    : prefix_(prefix) {}

SessionId PlayerSimpleIdGenerator::NewSessionId() {
  return SessionId(prefix_ + "-" + std::to_string(next_player_id_++));
}
