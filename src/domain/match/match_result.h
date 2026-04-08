#pragma once
#include <vector>
#include "../player/player_id.h"

struct MatchResult {
  struct Entry {
    PlayerId player_id;
    int score;
  };
  std::vector<Entry> rankings;  // отсортировано по убыванию score
};
