#pragma once

#include "player_id.h"
#include <string>

class Player {
public:
  const PlayerId &GetId() const;
  const std::string &GetNickname() const;

private:
  PlayerId id_;
  std::string nickname_;
};
