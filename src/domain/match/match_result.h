#pragma once

#include "../player/player_id.h"
#include <vector>

// struct MatchId {
//   std::string id;
//   double duration = 0.0f;
//   std::string session_id;
//   uint32_t finished_at;
// };

struct MatchResult {
  // это итог матча для сохранения в репозиторий
  struct Entry {
    // MatchId match_id;
    PlayerId player_id;
    double distance;
    int passed_pipes;
  };
  std::vector<Entry> rankings; // отсортировано уже по distance
};
