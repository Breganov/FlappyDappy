// domain\game\world_snapshot.h
#pragma once

#include "../session/session_state.h"

#include <cstdint>
#include <iostream>
// #include <iterator>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

struct PlayerSnapshot {
  std::string player_id;
  double x;
  double y;
  double velocity_y;
  bool alive;
  int passed_pipes;
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

// ============================== inline methods ==============================

inline std::string_view ToString(SessionState state) {
  switch (state) {
  case SessionState::WaitingForPlayers:
    return "Waiting";
  case SessionState::Countdown:
    return "Countdown";
  case SessionState::InProgress:
    return "InProgress";
  case SessionState::Finished:
    return "Finished";
  }
  return "Unknown";
}

inline std::ostream &operator<<(std::ostream &out, const WorldSnapshot &ws) {
  out << "\n============ WorldSnapshot ============\n";
  out << "Session ID: " << ws.session_id << '\n';
  out << "Tick: " << ws.tick << '\n';
  out << "State: " << ToString(ws.state) << '\n';

  out << "\nPlayers: " << ws.players.size() << '\n';

  for (const auto &player : ws.players) {
    out << "\n  Player ID: " << player.player_id << '\n';
    out << "    x: " << player.x << '\n';
    out << "    y: " << player.y << '\n';
    out << "    velocity_y: " << player.velocity_y << '\n';
    out << "    alive: " << player.alive << '\n';
    out << "    passed_pipes: " << player.passed_pipes << '\n';
    out << "    distance: " << player.distance << '\n';
  }

  out << "\nPipes: " << ws.pipes.size() << '\n';
  for (const auto &pipe : ws.pipes) {
    out << "\n  Pipe:\n";
    out << "    x: " << pipe.x << '\n';
    out << "    gap_y: " << pipe.gap_y << '\n';
    out << "    gap_height: " << pipe.gap_height << '\n';
  }

  out << "========================================\n\n";
  return out;
}
