#pragma once

#include "../player/player_id.h"
#include <vector>

struct MatchResult {
  // это итог матча для сохранения в репозиторий
  struct Entry {
    /* Надо добавить сюда MatchId, метаданные Match: id, duration,
     * session_id, finished_at;
     */
    PlayerId player_id;
    double distance;
    int passed_pipes;
  };
  std::vector<Entry> rankings; // отсортировано уже по distance
};
