// domain\session\games_session.h
# pragma once

// #include <algorithm>
#include <chrono>
#include <cstdint>
#include <queue>
// #include <utility>
#include <vector>

#include "input_command.h"
#include "input_command.h"
#include "player_session_state.h"
#include "player_session_state.h"
#include "session_state.h"

#include "../game/collision_service.h"
#include "../game/obstacle_generator.h"
#include "../game/physics_config.h"
#include "../game/physics_engine.h"
#include "../game/pipe.h"
#include "../game/world_snapshot.h"
#include "../match/match_result.h"
#include "../session/session_id.h"

class GameSession {
public:
  GameSession(SessionId id, std::uint32_t seed);
  const SessionId& GetId() const;
  SessionState GetState() const;

  void AddPlayer(PlayerId player_id);
  void MarkPlayerReady(const PlayerId& palyer_id);
  void EnqueueInput(const InputCommand& command);

  void StartCountdown();
  void StartMatch();
  void Tick(std::chrono::milliseconds delta);

  bool IsFinished() const;
  WorldSnapshot BuildSnapshot() const;
  MatchResult BuildResult() const;

private:
  void ApplyInputs_();
  void UpdateBirds_(double dt);
  void UpdatePipes_(double dt);
  void DetectCollisions_();
  void UpdateScores_();
  void CheckFinishConditions_();

private:
  SessionId id_;
  SessionState state_ = SessionState::WaitingForPlayers;
  std::vector<PlayerSessionState> players_;
  std::vector<Pipe> pipes_;
  std::queue<InputCommand> pending_inputs_;
  std::uint32_t seed_;
  std::uint64_t current_tick_ = 0;
  ObstacleGenerator obstacle_generator_;
};
