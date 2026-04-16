#pragma once

#include "../session/session_state.h"

#include <cstdint>
#include <string>
#include <vector>

struct PlayerSnapshot {
  std::string player_id;
  double y;
  double velocity_y;
  bool alive;
  int score;
  double distance;
};

struct PipeSnapshot {
  double x;
  double gap_y;
  double gap_height;
};

struct WorldSnapshot {
  std::string session_id;
  std::uint64_t tick;
  SessionState state;
  std::vector<PlayerSnapshot> players;
  std::vector<PipeSnapshot> pipes;
};
