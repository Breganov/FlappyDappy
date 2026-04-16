// domain\session\games_session.h
#pragma once

#include <chrono>
#include <cstdint>
#include <queue>
#include <vector>

#include "input_command.h"
#include "player_session_state.h"
#include "session_state.h"

#include "../game/collision_service.h"
#include "../game/obstacle_generator.h"
#include "../game/physics_config.h"
#include "../game/physics_engine.h"
#include "../game/pipe.h"
#include "../game/world_snapshot.h"
#include "../match/match_result.h"
#include "../player/player_id.h"
#include "../session/session_id.h"

class GameSession {
public:
  GameSession(SessionId id, std::uint32_t seed, double gravity,
              double jump_velocity);
  const SessionId &GetId() const;
  SessionState GetState() const;

  void AddPlayer(PlayerId player_id);
  void MarkPlayerReady(const PlayerId &player_id);
  void EnqueueInput(const InputCommand &command);

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
  PhysicsConfig physics_config_;
  PhysicsEngine physics_engine_;
};
