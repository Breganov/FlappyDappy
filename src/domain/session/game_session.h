# pragma once

#include "session_state.h"
#include "player_session_state.h"
#include "../game/pipe.h"
#include "input_command.h"
#include <chrono>
#include <cstdint>
#include <queue>
#include <vector>

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
};
