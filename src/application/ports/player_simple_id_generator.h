#pragma once

#include "application/ports/id_generator.h"
#include "domain/session/session_id.h"
#include <cstdint>

class PlayerSimpleIdGenerator : public IIdGenerator {
public:
  explicit PlayerSimpleIdGenerator(std::string prefix = "player");
  SessionId NewSessionId() override;

private:
  std::string prefix_;
  std::uint64_t next_player_id_ = 1;
};
