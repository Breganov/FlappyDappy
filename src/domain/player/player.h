#pragma once

#include "domain/player/nickname.h"
#include "player_id.h"
#include <utility>

class Player {
public:
  Player(PlayerId id, Nickname nickname)
      : id_(std::move(id)), nickname_(std::move(nickname)) {}
  const PlayerId &GetId() const { return id_; }
  const Nickname &GetNickname() const { return nickname_; }

private:
  PlayerId id_;
  Nickname nickname_;
};
