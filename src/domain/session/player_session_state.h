// domain\session\player_session_state.h
#pragma once

#include "domain/game/bird_state.h" // для BirdState
#include "domain/player/player_id.h"

class PlayerSessionState {
public:
  PlayerSessionState(PlayerId player_id, BirdState bird)
      : player_id_(std::move(player_id)), bird_(std::move(bird)) {}

  const PlayerId &GetPlayerId() const { return player_id_; }
  const BirdState &GetBird() const { return bird_; }
  BirdState &GetBird() { return bird_; }

  bool IsReady() const { return is_ready_; }
  void MarkReady() { is_ready_ = true; }

  bool IsConnected() const { return is_connected_; }
  void MarkDisconnected() { is_connected_ = false; }
  void MarkConnected() { is_connected_ = true; }

private:
  PlayerId player_id_;
  BirdState bird_;
  bool is_ready_ = false;
  bool is_connected_ = true;
};
