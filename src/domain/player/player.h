#pragma once

#include "player_id.h"
#include <string>
#include <utility>

class Player {
public:
  Player(PlayerId id, std::string nickname)
      : id_(std::move(id)), nickname_(std::move(nickname)) {}
  const PlayerId &GetId() const { return id_; }
  const std::string &GetNickname() const { return nickname_; }

private:
  PlayerId id_;
  std::string nickname_;
};
