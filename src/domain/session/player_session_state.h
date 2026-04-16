#pragma once

#include "../../domain/player/player_id.h"
#include "../game/bird_state.h" // для BirdState

class PlayerSessionState {
public:
  PlayerSessionState(PlayerId player_id, BirdState bird);

  const PlayerId &GetPlayerId() const { return player_id_; }
  const BirdState &GetBird() const { return bird_; }
  BirdState &GetBird() { return bird_; }

  bool IsReady() const;
  void MarkReady();

  bool IsConnected() const;
  void MarkDisconnected();
  void MarkConnected();

private:
  PlayerId player_id_;
  BirdState bird_;
  bool if_ready_ = false;
  bool if_connected_ = true;
};
